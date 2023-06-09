//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "TDMAMac.h"

//Register_Class(TDMAMac);

using namespace inet;
using namespace inet::physicallayer;

Define_Module(TDMAMac);

void TDMAMac::initialize(int stage){
    MacProtocolBase::initialize(stage);

    slotCounter = -1;

    /* This initStages mechanism is used for scheduling
     * the initialization of modules and their submodules
     * in a specific order. Below are predefined stages:
     * */
    // for initializing local parameters that do not depend on module's position in the network
    // module id, reading a configuration file, etc
    if(stage == INITSTAGE_LOCAL){
        bitrate = par("bitrate");
        slotDuration = par("slotDuration");

        txQueue = check_and_cast<queueing::IPacketQueue *>(getSubmodule("queue"));
    }
    //setting up communication medium, antennas
    /*else if(stage == INITSTAGE_PHYSICAL_LAYER){

    }*/
    else if(stage == INITSTAGE_LINK_LAYER){

        _timeout = new cMessage("_TIMEOUT");
        slotBeginsEvent = new cMessage("SLOT_BEGINS");

        topologyInfo.setFrameLength(par("frameLength").intValue());

        cModule *radioModule = getModuleFromPar<cModule>(par("radioModule"), this);
        //whenever the radio mode changes, handleRadioModeChanged() function of the current module will be called.
        radioModule->subscribe(IRadio::radioModeChangedSignal, this);
        //whenever the transmission state of radio changes, the handleTransmissionStateChanged() function of the current module will be called.
        radioModule->subscribe(IRadio::transmissionStateChangedSignal, this);
        //radio variable is assigned a pointer to the radio module,
        // which can be used to access the radio functionalities.
        radio = check_and_cast<IRadio *>(radioModule);
        // setting radio mode to Rx
        radio->setRadioMode(IRadio::RADIO_MODE_RECEIVER);

        fsm.setState(RECEIVE);

        // assign slots that given in configuration file while network is up
        std::string slotNumbers = par("slotNumbers");
        std::stringstream tokenizer(slotNumbers);
        std::string token;

        while (std::getline(tokenizer, token, ',')) {
            int slotNumber = std::stoi(token);
            slotNumbersVector.push_back(slotNumber);
            topologyInfo.setSlotAssignment(slotNumber, getContainingNode(this)->getId());
        }

        // scheduling self event for the current slot
        scheduleAt(simTime(), slotBeginsEvent);
    }
}

/*void TDMAMac::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}*/

void TDMAMac::handleSelfMessage(cMessage *msg){
    address = interfaceEntry->getMacAddress();

    if (msg != slotBeginsEvent){
        EV << "Unexpected self message type --- discarding " << endl;
        delete msg;
        return;
    }

    // scheduling self event for the next slot
    scheduleAt(simTime() + slotDuration, slotBeginsEvent);

    // increase slot number with 1 after receiving _slotBegins self message
    updateTimeSlotCounter(slotCounter);

    handleWithFsm(msg);

    printAssignedTimeSlots();

    sendPacketIfNodeAvailableForTX();
    //topologyInfo.printSlotAssignment();
}

void TDMAMac::handleUpperPacket(Packet *packet){
    encapsulate(packet);
    EV << getContainingNode(this)->getFullName() << " received a message from upper layer, name is " << packet->getName() << ", dest mac addr=" << packet->peekAtFront<TDMAMacHeaderBase>()->getDestAddr() << endl;
    txQueue->pushPacket(packet);
}

void TDMAMac::handleLowerPacket(Packet *packet){
    EV << getContainingNode(this)->getFullName() << " received a message from lower layer: " << packet->getName() << endl;
    //handleWithFsm(packet);

    if (packet->hasBitError()){
        //EV << "Received " << packet << " contains bit errors or collision, dropping it\n";
        PacketDropDetails details;
        details.setReason(INCORRECTLY_RECEIVED);
        emit(packetDroppedSignal, packet, &details);
        delete packet;
        return;
    }

    const auto& hdr = packet->peekAtFront<TDMAMacHeaderBase>();
    packet->setKind(hdr->getType());

    EV << "Received packet type " << packet->getFullName() << endl;
    // to send the packet to upper layers
    decapsulate(packet);
    sendUp(packet);
}

void TDMAMac::encapsulate(Packet *packet){
    auto pkt = makeShared<TDMAMacDataFrame>();
    pkt->setChunkLength(b(par("headerLength")));

    auto dest = packet->getTag<MacAddressReq>()->getDestAddress();
    pkt->setDestAddr(dest);
    pkt->setNetworkProtocol(ProtocolGroup::ethertype.getProtocolNumber(packet->getTag<PacketProtocolTag>()->getProtocol()));

    delete packet->removeControlInfo();                        // delete the control info

    pkt->setSrcAddr(interfaceEntry->getMacAddress());

    pkt->setType(TDMAMAC_DATA);
    //encapsulate the network packet
    packet->insertAtFront(pkt);
    packet->getTag<PacketProtocolTag>()->setProtocol(&Protocol::tdmaMac);

    EV_DETAIL << "message encapsulated\n";
}

void TDMAMac::decapsulate(Packet *packet){
    const auto& macHeader = packet->popAtFront<TDMAMacDataFrame>();
    packet->addTagIfAbsent<MacAddressInd>()->setSrcAddress(macHeader->getSrcAddr());
    packet->addTagIfAbsent<InterfaceInd>()->setInterfaceId(interfaceEntry->getInterfaceId());
    auto payloadProtocol = ProtocolGroup::ethertype.getProtocol(macHeader->getNetworkProtocol());
    packet->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(payloadProtocol);
    packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(payloadProtocol);
    EV_DETAIL << " message decapsulated " << endl;
}

void TDMAMac::receiveSignal(cComponent *source, simsignal_t signalID, intval_t value, cObject *details){
    //Enter_Method_Silent();
    if (signalID == IRadio::transmissionStateChangedSignal){
        IRadio::TransmissionState newRadioTransmissionState = static_cast<IRadio::TransmissionState>(value);
        if (transmissionState == IRadio::TRANSMISSION_STATE_TRANSMITTING && newRadioTransmissionState == IRadio::TRANSMISSION_STATE_IDLE){
            if (_timeout->isScheduled())                        // if _timeout scheduled by invoking of scheduleAt() function
                cancelEvent(_timeout);                          // then cancel the _timeout
        }
        transmissionState = newRadioTransmissionState;
    }
}

void TDMAMac::attachSignal(Packet *macPkt){
    //calc signal duration
    simtime_t duration = macPkt->getBitLength() / bitrate;
    //create and initialize control info with new signal
    macPkt->setDuration(duration);
}

TDMAMac::~TDMAMac(){
    cancelAndDelete(_timeout);
    cancelAndDelete(slotBeginsEvent);
}

void TDMAMac::configureInterfaceEntry(){
    MacAddress address = parseMacAddressParameter(par("address"));
    interfaceEntry->setDatarate(bitrate);               // data rate

    // generate a link-layer address to be used as interface token for IPv6
    interfaceEntry->setMacAddress(address);
    interfaceEntry->setInterfaceToken(address.formInterfaceIdentifier());

    // capabilities
    interfaceEntry->setMtu(par("mtu"));
    interfaceEntry->setMulticast(false);
    interfaceEntry->setBroadcast(true);
}

// sends Data Frame
void TDMAMac::sendPacket(){
    //radio->setRadioMode(IRadio::RADIO_MODE_TRANSMITTER);                // setting radio mode to transmitter
    currentTxFrame = txQueue->popPacket();
    //take(currentTxFrame);                                               // to take the ownership of the packet
    // auto hdr = currentTxFrame->peekAtFront<TDMAMacHeaderBase>();
    sendDown(currentTxFrame->dup());

}

void TDMAMac::updateTimeSlotCounter(int &slotCounter){
    slotCounter++;
    if(slotCounter == par("frameLength").intValue()){
        EV << "New frame with length " << par("frameLength").intValue() << " has just started.\n";
        slotCounter = 0;
    }
    EV << "Slot: [" << slotCounter << "]" << std::endl;
}

void TDMAMac::printAssignedTimeSlots(){
    EV << "Assigned time slots of " << getContainingNode(this)->getFullName() << ": ";
    for (int slot : slotNumbersVector) {
        EV << slot << " ";
    }
    EV << endl;
}

bool TDMAMac::isMyTimeSlot(int slotNum) const {
    int currentNodeId = topologyInfo.getSlotAssignment(slotNum);

    if (currentNodeId == getContainingNode(this)->getId()) {
        return true;
    }
    else if(currentNodeId == -1 ) {
        return false;
    }
}

void TDMAMac::sendPacketIfNodeAvailableForTX(){
    switch (radio->getRadioMode()) {
        case IRadio::RADIO_MODE_TRANSMITTER:
            if (!txQueue->isEmpty()) {
                sendPacket();
            } else {
                EV << "Queue is empty\n";
            }
            break;
        default:
            EV << "Radio mode is not transmitter mode\n";
            break;
    }

}

void TDMAMac::handleWithFsm(cMessage *msg){

    FSMA_Switch(fsm){

        FSMA_State(RECEIVE){
            FSMA_Event_Transition(Start-Transmit,
                                  isMyTimeSlot(slotCounter),
                                  TRANSMIT, radio->setRadioMode(IRadio::RADIO_MODE_TRANSMITTER);
            );
        }

        FSMA_State(TRANSMIT){
           // FSMA_Enter(sendPacketIfNodeAvailableForTX());

            FSMA_Event_Transition(Start-Receive,
                                  !(isMyTimeSlot(slotCounter)),
                                  RECEIVE,  radio->setRadioMode(IRadio::RADIO_MODE_RECEIVER);
            );
        }
    }



}
