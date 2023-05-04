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

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedVisualizer;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.node.inet.INetworkNode;

import inet.node.inet.WirelessHost;

network Network
{
    @display("bgb=799,400;bgi=maps/world");
    @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=60,231");
        }
        visualizer: IntegratedVisualizer {
            @display("p=60,302");
        }
        radioMedium: UnitDiskRadioMedium {
            @display("p=60,364");
        }

        host1: SimpleNode {
            @display("p=516,90");
        }
        host2: SimpleNode {
            @display("p=448,150");
        }
        host3: SimpleNode {
            @display("p=442,110");
        }
        host4: SimpleNode {
            @display("p=530,139");
        }
        host5: SimpleNode {
            @display("p=183,110");
        }



}
