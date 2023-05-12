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

#ifndef SRC_TIB_H_
#define SRC_TIB_H_

#include <omnetpp.h>
#include <vector>
#include <map>

using namespace omnetpp;
using namespace std;

/* topology information base
 * serves as a distributed knowledge base that
 * network components, management systems, and protocols can access
 * to gather network information and make informed decisions.
 *
 * It facilitates efficient network operations,
 * facilitates dynamic routing,
 *  supports network planning and optimization,
 *  and aids in network troubleshooting and fault management.
*/

class TIB {
private:
    int frameLength;
    std::vector<std::string> hostNames;
    std::vector<int> slotAssignment;

public:
    TIB();
    TIB(int frameLength);
    virtual ~TIB();

    int getFrameLength() const;
    void setFrameLength(int length);
    std::string getHostName(int index) const;

    int getSlotAssignment(int slotNum) const;
    void setSlotAssignment(int slotNum,int hostName);

    void printSlotAssignment() const;
};

#endif /* SRC_TIB_H_ */
