#ifndef __NETINET_IP_H__
#define __NETINET_IP_H__

/* REFERENCES
 * ----------
 * http://unix.superglobalmegacorp.com/Net2/newsrc/netinet/ip.h.html
 * http://fxr.watson.org/fxr/source/netinet/ip.h
 */

/* internet implementation parameters */

#define IP_V4                      4
#define IP_V6                      6

#define MAXTTL                     255  // maximum time to live
#define IPDEFTTL                   64   // default TTL; RFC 1340
#define IPFRAGTTL                  60   // time to live for frags
#define IPTTLDEC                   1    // subtracted when forwarding
#define IP_MMS                     576  // default maximum segment size

#include <stdint.h>
#include <endian.h>

#include <zero/pack.h>

/*
 * internet header structure without options
 * - ip_len and ip_off are signed for comparisons not to fail
 */

/* defnitions for DiffServ Codepoints; RFC2474, RFC5865 */
#define IPTOS_DSCP_CS0             0x00
#define IPTOS_DSCP_CS1             0x20
#define IPTOS_DSCP_AF11            0x28
#define IPTOS_DSCP_AF12            0x30
#define IPTOS_DSCP_AF13            0x38
#define IPTOS_DSCP_CS2             0x40
#define IPTOS_DSCP_AF21            0x48
#define IPTOS_DSCP_AF22            0x50
#define IPTOS_DSCP_AF23            0x58
#define IPTOS_DSCP_CS3             0x60
#define IPTOS_DSCP_AF31            0x68
#define IPTOS_DSCP_AF32            0x70
#define IPTOS_DSCP_AF33            0x78
#define IPTOS_DSCP_CS4             0x80
#define IPTOS_DSCP_AF41            0x88
#define IPTOS_DSCP_AF42            0x90
#define IPTOS_DSCP_AF43            0x98
#define IPTOS_DSCP_CS5             0xa0
#define IPTOS_DSCP_VA              0xb0
#define IPTOS_DSCP_EF              0xb8
#define IPTOS_DSCP_CS6             0xc0
#define IPTOS_DSCP_CS7             0xe0

#define IPVERSION                  4
#define IPMAXPACKET                65535
/* ip_tos-field type of service bits */
#define IPTOS_LOWDELAY             0x10
#define IPTOS_THROUGHPUT           0x08
#define IPTOS_RELIABILITY          0x04
#define IPTOS_MINCOST              0x02
/* ip_tos-field IP precedence flags */
#define IPTOS_PREC_NETCONTROL      IPTOS_DSACP_CS7
#define IPTOS_PREC_INTERNETCONTROL IPTOS_DSCP_CS6
#define IPTOS_PREC_CRITIC_ECP      IPTO_DSCP_CS5
#define IPTOS_PREC_FLASHOVERRIDE   IPTOS_DSCP_CS4
#define IPTOS_PREC_FLASH           IPTOS_DSCP_CS3
#define IPTOS_PREC_IMMEDIATE       IPTOS_DSCP_CS2
#define IPTOS_PREC_PRIORITY        IPTOS_DSCP_CS1
#define IPTOS_PREC_ROUTINE         IPTOS_DSCP_CS0
/* ECN (explicit congestion notification); RFC3168, low 2 bits of ip_tos */
#define IPTOS_ECN_NOTECT           0x00 // not ECT
#define IPTOS_ECN_ECT1             0x01 // ECN-capable transport (1)
#define IPTOS_ECN_ECT0             0x02 // ECN-capable transport (0)
#define IPTOS_ECN_CE               0x03 // congestion experienced
#define IPTOS_ECN_MASK             0x03 // ECN-field mask
/* ip_off field flag-bits */
#define IP_RF                      0x8000 // reserved frament flag
#define IP_DF                      0x4000 // don't fragment bit
#define IP_MF                      0x2000 // more fragments bit
#define IP_OFFMASK                 0x1fff // offset mask
struct ip {
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    unsigned       ip_hl : 4;   // header length
    unsigned       ip_v  : 4;   // version
#elif (__BYTE_ORDER == __BIG_ENDIAN)
    unsigned       ip_v  : 4;   // version
    unsigned       ip_hl : 4;   // header length
#endif
    uint8_t        ip_tos;      // type of service
    int16_t        ip_len;      // total length
    uint16_t       id_id;       // identification
    int16_t        ip_off;      // fragment offset
    uint8_t        ip_ttl;      // time to live
    uint8_t        ip_p;        // protocol
    uint16_t       ip_sum;      // checksum
    struct in_addr ip_src;      // source address
    struct in_addr ip_dst;      // destination address
} PACKED();

#include <zero/nopack.h>

/* option definitions */
#define IPOPT_COPIED(o)        ((o) & 0x80)
#define IPOPT_CLASS(o)         ((o) & 0x60)
#define IPOPT_NUMBER(o)        ((o) & 0x1f)
#define IPOPT_CONTROL          0x00
#define IPOPT_RESERVED1        0x20
#define IPOPT_DEBMEAS          0x40
#define IPOPT_RESERVED2        0x60
#define IPOPT_EOL              0x00     // end of option list
#define IPOPT_NOP              0x01     // no operation
#define IPOPT_RR               0x07     // record packet route
#define IPOPT_TS               0x44     // timestamp
#define IPOPT_SECURITY         0x82     // provide s, c, h, tcc */
#define IPOPT_LSRR             0x83     // loose source route
#define IPOPT_ESO              0x85     // extended security
#define IPOPT_CIPSO            0x86     // commercial security
#define IPOPT_SATID            0x88     // satnet ID
#define IPOPT_SSRR             0x89     // strict source route
#define IPOPT_RA               0x94     // router alert
/* offset fields in options other than EOL and NOP */
#define IPOPT_OPTVAL           0x00     // option ID
#define IPOPT_OLEN             0x01     // option length
#define IPOPT_OFFSET           0x02     // offset within option
#define IPOPT_MINOFF           0x04     // minimum offset

/* timestamp option structure */

/* ipt_flg-values */
#define IPOPT_TS_TSONLY        0x00     // timestamp only
#define IPOPT_TS_TSANDADDR     0x01     // timestamp and address
#define IPOPT_TS_PRESPEC       0x03     // specified modules
/* security-flags (not byte-swapped) */
#define IPOPT_SECUR_UNCLASS    0x0000
#define IPOPT_SECUR_CONFID     0xf135
#define IPOPT_SECUR_EFTO       0x789a
#define IPOPT_SECUR_MMMM       0xbc4d
#define IPOPT_SECUR_RESTR      0xaf13
#define IOPPT_SECUR_SECRET     0xd788
#define IPOPT_SECUR_TOPSECRET  0x6bc5
struct ip_timestamp {
    uint8_t                ipt_code;            // IPOPT_TS
    uint8_t                ipt_len;             // structure size
    uint8_t                ipt_ptr;             // current entry index
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    unsigned               ipt_flg  : 4;        // flags
    unsigned               ipt_oflw : 4;        // overflow counter
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
    unsigned               ipt_oflw : 4;        // overflow counter
    unsigned               ipt_flg  : 4;        // flags
#endif
    union ipt_timestamp {
        uint32_t           ipt_time;
        struct   ipt_ta {
            struct in_addr ipt_addr;
            uint32_t       ipt_time;
        } ipt_ta[1];
    } ipt_timestamp;
};

/*
 * IPv4 pseudo header for computing TCP and UDP checksums
 * - use struct ipovly for Internet checksums
 * - for strong checksums, use struct ip
 */
struct ippseudo {
    struct in_addr ippseudo_src;        // source address
    struct in_addr ippseudo_dst;        // destination address
    uint8_t        ippseudo_pad;        // pad - MUST be zero
    uint8_t        ippseudo_p;          // protocol
    uint16_t       ippseudo_len;        // protocol length
};

#endif /* __NETINET_IP_H__ */

