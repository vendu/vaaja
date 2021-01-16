#ifndef __NETINET_UDP_H__
#define __NETINET_UDP_H__

#include <stdint.h>

struct udphdr {
    uint16_t uh_sport;  // source port
    uint16_t uh_dport;  // destination port
    int16_t  uh_ulen;   // length
    uint16_t uh_sum;    // UDP checksum
};

#endif /* __NETINET_UDP_H__ */

