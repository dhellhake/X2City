using PcapDotNet.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TraceView.Model
{
    public class EthSocket
    {

        public EthSocket()
        {
            PacketCommunicator communicator;
            // Retrieve the interfaces list
            IList<LivePacketDevice> allDevices = LivePacketDevice.AllLocalMachine;

            LivePacketDevice eth0 = null;
            // Scan the list printing every entry
            for (int i = 0; i != allDevices.Count(); ++i)
                foreach (DeviceAddress addr in allDevices[i].Addresses)
                    if (addr.Address.ToString().Contains("192.168.1.1"))
                        eth0 = allDevices[i];

            // Open the device
            communicator = eth0.Open(65536, PacketDeviceOpenAttributes.Promiscuous, 1000);
        }
    }
}
