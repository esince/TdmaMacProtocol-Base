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

#ifndef __PLAINPROJECT_TDMAMAC_H_
#define __PLAINPROJECT_TDMAMAC_H_

#include <omnetpp.h>
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/linklayer/contract/IMacProtocol.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/linklayer/common/InterfaceTag_m.h"       // InterfaceINd, decapsulate
#include "inet/common/ProtocolGroup.h"                  // ProtocolGroup, decapsulate, encapsulate
#include "inet/common/ProtocolTag_m.h"                  // PacketProtocolTag, addTag, addTagIfAbsent, getTag
#include "inet/physicallayer/contract/packetlevel/SignalTag_m.h"
#include "inet/common/ModuleAccess.h"                   // getContainingNOde(), getModuleFromPar
#include "inet/linklayer/common/MacAddressTag_m.h"      // MacAddressInd
#include "inet/common/FSMA.h"

#include "TDMAMacHeader_m.h"
#include "TIB.h"

using namespace omnetpp;
using namespace inet;

class TDMAMac : public MacProtocolBase, public IMacProtocol
{
  protected:
    virtual void configureInterfaceEntry() override;

    void scheduleTimeSlots();
    void updateTimeSlotCounter(int &currentSlotCounter);
    bool isMyTimeSlot(int slotNum) const;
    void sendPacketIfNodeAvailableForTX();
    void printAssignedTimeSlots();

    int slotCounter;
    std::vector<int> slotNumbersVector;

    TIB topologyInfo;


    enum State {
        RECEIVE,
        TRANSMIT,
    };
    cFSM fsm;

    double slotDuration;
    double bitrate;

    MacAddress address;

    cMessage *slotBeginsEvent;
    cMessage *_timeout;

    queueing::IPacketQueue *txQueue;

    physicallayer::IRadio *radio;
    physicallayer::IRadio::TransmissionState transmissionState;
private:
public:

    TDMAMac() : MacProtocolBase(),
            slotCounter(-1), bitrate(0),
            slotDuration(0), radio(nullptr),
            transmissionState(physicallayer::IRadio::TRANSMISSION_STATE_UNDEFINED),
            _timeout(nullptr), slotBeginsEvent(nullptr)
            {}
    virtual ~TDMAMac();
    virtual void initialize(int stageNum) override;
   // virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage *) override;
    virtual void handleUpperPacket(Packet *packet) override;
    virtual void handleLowerPacket(Packet *packet) override;
    virtual void handleWithFsm(cMessage *msg);

    virtual void receiveSignal(cComponent *source, simsignal_t signalID, intval_t value, cObject *details) override;
    void attachSignal(Packet *macPkt);

    virtual void encapsulate(Packet *);
    virtual void decapsulate(Packet *);

    void sendPacket();

};

#endif
