# Flud; Packet Filter Programming

- flud reminds of exiting filter programming systems auch as bfs (berkeley packet filters)
- flud programs operate in a context of a packet; if a filter returns zero, the packet is dropped;
  otherwise, if the return value is less than the packet size, it's truncated to returned length;
  finally, the whole packet is allowed to proceed unmodified

## Flud API

- instead of arbitrarily-named variables, we use 'registers' R0..R8 to process
  network packets
- when a flud program is invoked, it's passed the following
  - R0   - packet buffer address (first core word-slot) - alias PKT
  - R1   - packet size in bytes (including header)      - alias PKTSIZE
  - R2   -

uint32_t gettm();               // read packet-timestamp from header
uint32_t getreqtm();            // read original request-timestamp from header
uint32_t getsrc();              // read source address from header
uint32_t getdst();              // read destination address from header
uint16_t getsrcport();          // read source port from header
uint16_t getdstport();          // read destination port from header
uint16_t getsize();             // read packet-size from header
uint8_t  getfrag();             // read fragment ID from header
uint8_t  getflags();            // read packet-flags from header
uint8_t  getttl();              // read time-to-live from packet header
uint8_t  gethops();             // read hop-count from packet header
uint16_t getcrc();              // read crc from packet header
uint16_t getsrv();              // read service ID for local server/program/process
uint16_t getdata(buf, nb);      // read nb bytes from data field (-1 -> all);
                                // return # of bytes read
#define PKT_REQ_FLAG            (1 << 0)        // request (as opposed to reply)
#define PKT_ECHO_FLAG           (1 << 1)        // ask the packet to be echoed back verbatim
#define PKT_MCAST_FLAG          (1 << 2)        // multicast packet
#define PKT_PING_FLAG           (1 << 3)        // ping request
#define PKT_TRACE_FLAG          (1 << 4)        // ask routers to report their addresses back
#define PKT_ACK_FLAG            (1 << 5)        // acknowledge received packet (PCP)
#define PKT_SYN_FLAG            (1 << 6)        // synchronize transmission
#define PKT_FIN_FLAG            (1 << 7)        // finish transmission

struct pktmsg {
00     uint32_t tm;             // timestamp
04     uint32_t reqtm;          // request ID for replies, i.e. original request timestamp
08     uint8_t  src[4];         // source address
0b     uint8_t  dst[4];         // destination address
10     uint16_t srcport;        // source port
12     uint16_t dstport;        // destination port
14     uint16_t size;           // packet size in 32-bit words (including 32-byte header)
16     uint8_t  frag;           // fragment ID
17     uint8_t  flags;          // REQ, ECHO, MCAST, PING, TRACE, ACK, SYN, FIND
18     uint8_t  ttl;            // time-to-live in hops
1a     uint8_t  hops;           // number of hops so far
1b     uint16_t crc             // checksum
1d     uint16_t srv;            // origin service ID
20     data[];                  // packet data
};

# Standard Protocols

- PNP - packet network protocol
  - low-level protocol on top of which PCP, PDP, and PTP are implement
- PCP - packet control protocol
- PDP - packet datagram protocol
- PTP - packet transfer protocol

# Standard services

- DIAL
- LOGIN
- XFER

# Flud Filter Environment

## Packet Queues
- INPUT
- OUTPUT

### Queue Policies
- ACCEPT
- LOG
- REJECT
- DROP
- FORWARD
- MAP
- MASQ
- ROUTE

## Packet Structure

#define PKT_REQ_FLAG            (1 << 0)        // request (as opposed to reply)
#define PKT_ECHO_FLAG           (1 << 1)        // ask the packet to be echoed back verbatim
#define PKT_MCAST_FLAG          (1 << 2)        // multicast packet
#define PKT_PING_FLAG           (1 << 3)        // ping request
#define PKT_TRACE_FLAG          (1 << 4)        // ask routers to report their addresses back
#define PKT_ACK_FLAG            (1 << 5)        // acknowledge received packet (PCP)
#define PKT_SYN_FLAG            (1 << 6)        // synchronize transmission
#define PKT_FIN_FLAG            (1 << 7)        // finish transmission

struct pktmsg {
00     uint32_t tm;             // timestamp
04     uint32_t reqtm;          // request ID for replies, i.e. original request timestamp
08     uint8_t  src[4];         // source address
0b     uint8_t  dst[4];         // destination address
10     uint16_t srcport;        // source port
12     uint16_t dstport;        // destination port
14     uint16_t size;           // packet size in 32-bit words (including 32-byte header)
16     uint8_t  frag;           // fragment ID
17     uint8_t  flags;          // REQ, ECHO, MCAST, PING, TRACE, ACK, SYN, FIND
18     uint8_t  ttl;            // time-to-live in hops
1a     uint8_t  hops;           // number of hops so far
1b     uint16_t crc             // checksum
1d     uint16_t srv;            // origin service ID
20     data[];                  // packet data
};

uint32_t gettm();               // read packet-timestamp from header
uint32_t getreqtm();            // read original request-timestamp from header
uint32_t getsrc();              // read source address from header
uint32_t getdst();              // read destination address from header
uint16_t getsrcport();          // read source port from header
uint16_t getdstport();          // read destination port from header
uint16_t getsize();             // read packet-size from header
uint8_t  getfrag();             // read fragment ID from header
uint8_t  getflags();            // read packet-flags from header
uint8_t  getttl();              // read time-to-live from packet header
uint8_t  gethops();             // read hop-count from packet header
zuint16_t getcrc();              // read crc from packet header
uint16_t getsrv();              // read service ID for local server/program/process
uint16_t getdata(buf, nb);      // read nb bytes from data field (-1 -> all);
                                // return # of bytes read

# Standard Protocols

- PNP - packet network protocol
  - low-level protocol on top of which PCP, PDP, and PTP are implement
- PCP - packet control protocol
- PDP - packet datagram protocol
- PTP - packet transfer protocol

# Standard services

- DIAL
- LOGIN
- XFER

# PKT Filter Language

## Packet Queues
- INPUT
- OUTPUT

### Queue Policies
- ACCEPT
- LOG
- REJECT
- DROP
- FORWARD
- MAP
- MASQ
- ROUTE

