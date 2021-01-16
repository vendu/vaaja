#ifndef __NETINET_IN_H__
#define __NETINET_IN_H__

#include <inttypes.h>
#include <endian.h>
#include <sys/socket.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>

#define __INADDRSZ          4
#define __IN6ADDRSZ         16

#define INET_ADDRSTRLEN     16
#define INET6_ADDRSTRLEN    46

#define IPPROTO_RAW         1   // raw IP packets
#define IPPROTO_ICMP        2   // control message
#define IPPROTO_TCP         3   // transmission control protocol
#define IPPROTO_IP          4   // internet protocol
#define IPPROTO_UDP         5   // user datagram prtocol
#define IPPROTO_IPV6        6   // internet protocol version 6

#define IPV6_JOIN_GROUP     1   // join a multicast group
#define IPV6_LEAVE_GROUP    2   // leave a multicast group
#define IPV6_MULTICAST_HOPS 3   // multicast hop limit
#define IPV6_MULTICAST_IF   4   // interface for outgoing multicast packets
#define IPV6_MULTICAST_LOOP 5   // loo multicast packets back to application

#define IPV6_UNICAST_HOPS   6   // unicast hop limit
#define IPV6_V6ONLY         7   // restrict AF_INET6 socket to IPv6 only

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr {
    in_addr_t s_addr;
};

#define sin_zero _res           // compatibility with older systems
struct sockaddr_in {
    sa_family_t    sin_family;
    socklen_t      sin_len;
    in_port_t      sin_port;
    struct in_addr sin_addr ALIGNED(CLSIZE);
    uint8_t        _res[rounduppow2(SOCK_MAXADDRLEN, CLSIZE)
                        - sizeof(struct in_addr)];
};

#define INADDR_ANY       { 0 }
#define INADDR_BROADCAST { 0xffffffffU }
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define INADDR_LOOPBACK  { 0x0100007fU }
#else
#define INADDR_LOOPBACK  { 0x7f000001U }
#endif

struct ip_mreqn {
    struct in_addr imr_multiaddr;       // multicast group address
    struct in_addr imr_address;         // local interface address
    int            imr_ifindex;         // interface index
};

struct ip_mreq_source {
    struct in_addr imr_multiaddr;       // multicast group address
    struct in_addr imr_interface;       // local interface address
    struct in_addr imr_sourceaddr;      // multicast source address
};

#define MCAST_EXCLUDE 0                 // block sources on imsf_slist
#define MCAST_INCLUDE 1                 // allow sources on imsf_slist
/* allocation size for struct ip_msfilter with n imsf_slist entries */
#define IP_MSFILTER_SIZE(n)                                             \
    (offsetof(struct ip_msfilter, imsf_slist) + (n) * sizeof(struct in_addr))
struct ip_msfilter {
    struct in_addr imsf_multiaddr;
    struct in_addr imsf_interface;
    uint32_t       imsf_fmode;
    uint32_t       imsf_numsrc;
    struct in_addr imsf_slist[1];
};

#define IN6ADDR_ANY_INIT { 0 }  // all 0-bits
#define IN6ADDR_LOOPBACK_INIT                                           \
    { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } }
struct in6_addr {
    uint8_t s6_addr[16];        // network byte-order
};

/* NOTE: sin6_port and sin6_addr are in network byte-order */
struct sockaddr_in6 {
    sa_family_t     sin6_family;        // AF_INET6
    socklen_t       sin6_len;           // address size
    in_port_t       sin6_port;          // port #
    int8_t          _res[2];            // pad to 64-bit boundary
    uint32_t        sin6_flowinfo;      // traffic class and flow information
    uint32_t        sin6_scope_id;      // scope ID
    struct in6_addr sin6_addr ALIGNED(CLSIZE);
    uint8_t         _res2[rounduppow2(SOCK_MAXADDRLEN, CLSIZE)
                          - sizeof(struct in6_addr)];
};

struct ipv6_mreq {
    struct in6_addr ipv6mr_multiaddr;   // IPv6 multicast address
    unsigned int    ipv6mr_interface;
};

const struct in6_addr in6addr_any;
const struct in6_addr in6addr_loopback;

#endif /* __NETINET_IN_H__ */

