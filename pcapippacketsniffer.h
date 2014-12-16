#ifndef PCAPIPPACKETSNIFFER_H
#define PCAPIPPACKETSNIFFER_H

#include "headers.h"
#include "ippacketsnifferbase.h"

namespace HttpSniffer
{

class PcapIpPacketSniffer : public IpPacketSnifferBase
{
public:
    PcapIpPacketSniffer();

    void init();

    IpPacket get_ip_packet();
};

}

#endif // PCAPIPPACKETSNIFFER_H
