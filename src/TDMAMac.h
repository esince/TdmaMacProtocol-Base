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

using namespace omnetpp;
using namespace inet;


/**
 * TODO - Generated class
 */
class TDMAMac : public MacProtocolBase, public IMacProtocol
{
  protected:
    virtual void configureInterfaceEntry() override;
    void scheduleTimeSlots();
    void updateTimeSlotCounter(int &currentSlotCounter);
    int slotCounter;
    double slotDuration;
    double bitrate;
    MacAddress address;
    cMessage *_slotBegins;
    cMessage *_timeout;
private:
public:

    TDMAMac() : MacProtocolBase(),
            slotCounter(-1), bitrate(0), slotDuration(0)
            {}
    virtual ~TDMAMac();
    virtual void initialize(int) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage *) override;

    virtual void encapsulate(Packet *);
    virtual void decapsulate(Packet *);


};

#endif
