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

#include "TIB.h"

TIB::TIB() {
    // TODO Auto-generated constructor stub

}
// copy constructor
TIB::TIB(int x) {
    // TODO Auto-generated constructor stub
    this->frameLength = frameLength;
    slotAssignment.resize(frameLength);

}

TIB::~TIB() {
    // TODO Auto-generated destructor stub
}

int TIB::getFrameLength() const {
    return frameLength;
}

void TIB::setFrameLength(int length) {
    frameLength = length;
    slotAssignment.resize(frameLength);
}

std::string TIB::getHostName(int index) const {
    if (index >= 0 && index < hostNames.size()) {
        return hostNames[index];
    } else {
        return "";
    }
}

int TIB::getSlotAssignment(int slotNum) const {
    static int emptySlot = -1;
        if (slotNum >= 0 && slotNum < slotAssignment.size()) {
            return slotAssignment[slotNum];
        } else {
            return emptySlot;
        }
}

void TIB::setSlotAssignment(int slotNum, int hostName) {

    if (slotNum >= 0 && slotNum < slotAssignment.size()) {
            slotAssignment[slotNum] = hostName;
        }
}

void TIB::printSlotAssignment() const {
    EV << "Slot Assignment:" << endl;
    for (size_t i = 0; i < slotAssignment.size(); i++) {
        EV << "[" << i << "]" <<": HostId: " <<  slotAssignment[i] << endl;
    }
}

