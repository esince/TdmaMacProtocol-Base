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

package src;

import inet.queueing.contract.IPacketQueue;
import inet.linklayer.base.MacProtocolBase;
import inet.linklayer.contract.IMacProtocol;
//
// TODO auto-generated module
//
//simple TDMAMac
//{
//}

module TDMAMac extends MacProtocolBase like IMacProtocol
{
    parameters:
        @class(TDMAMac);
        string address = default("auto");
        string networkName = default("Network"); 
        string radioModule = default("^.radio");   // The path to the Radio module  //FIXME remove default value
        
        double slotDuration @unit(s) = default(100ms);
        int headerLength @unit(b) = default(10b);
        double defaultChannel = default(0);  // default channel
        double bitrate @unit(bps) = default(19200bps); 
        int mtu @unit(B) = default(0B);
        
        int assignedSlot = default(-1);
  	
   submodules:
        queue: <default("DropTailQueue")> like IPacketQueue {
            parameters:
                packetCapacity = default(20);
                @display("p=100,100;q=l2queue");
        }

        
}
