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
#include "inet/common/ModuleAccess.h"                   // getContainingNOde(), getModuleFromPar
#include "inet/linklayer/common/InterfaceTag_m.h"       // InterfaceINd, decapsulate

//Register_Class(TDMAMac);

using namespace inet;

Define_Module(TDMAMac);

void TDMAMac::initialize(int stage)
{

    MacProtocolBase::initialize(stage);
    EV_DETAIL << "Simple module initialized\n";
    slotCounter = -1;
    EV_DETAIL << slotCounter << endl;
    updateTimeSlotCounter(slotCounter);

    /* This initStages mechanism is used for scheduling
     * the initialization of modules and their submodules
     * in a specific order. Below are predefined stages:
     * */
    // for initializing local parameters that do not depend on module's position in the network
    // module id, reading a configuration file, etc
    if(stage == INITSTAGE_LOCAL){
        bitrate = par("bitrate");                       // transmission bitrate
        slotDuration = par("slotDuration");             // duration of a slot
    }
    //setting up communication medium, antennas
    /*else if(stage == INITSTAGE_PHYSICAL_LAYER){

    }*/
    else if(stage == INITSTAGE_LINK_LAYER){
        _timeout = new cMessage("_TIMEOUT");
        _slotBegins = new cMessage("_SLOT_BEGINS");

        EV_DETAIL << " slotDuration = " << slotDuration << endl;

        // scheduling self event for the current slot
        scheduleAt(simTime(), _slotBegins);
    }

}

void TDMAMac::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void TDMAMac::handleSelfMessage(cMessage *msg){
    address = interfaceEntry->getMacAddress();
    EV << "Unexpected self message type --- discarding " << endl;
    /* if (msg->getKind() != _SLOT_BEGINS){
        EV << "Unexpected self message type --- discarding " << endl;
        return;
    }*/
    // scheduling self event for the next slot
    scheduleAt(simTime() + slotDuration, _slotBegins);
    // increase slot number with 1 after receiving _slotBegins self message
    updateTimeSlotCounter(slotCounter);
}
void TDMAMac::encapsulate(Packet *packet)
{
   // auto macHeader = makeShared<TDMAMacHeader>();
    //macHeader->setChunkLength(B(headerLength));
}
void TDMAMac::decapsulate(Packet *packet)
{

}

TDMAMac::~TDMAMac(){
    cancelAndDelete(_timeout);
    cancelAndDelete(_slotBegins);
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

void TDMAMac::updateTimeSlotCounter(int &slotCounter){
    slotCounter++;
    // TODO - If end of a network cycle
    EV << "Slot: [" << slotCounter << "]" << std::endl;
}
