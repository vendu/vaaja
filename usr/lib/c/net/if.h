#ifndef __NET_IF_H__
#define __NET_IF_H__

#define NEWIFQUEUE 1

#include <features.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <zero/param.h>
#include <kern/mem/mem.h>

#define IF_NAMESIZE 16  // includes terminating NUL
struct if_nameindex {
    unsigned int  if_index;
    char         *if_name;      // e.g. "eth0"
};

#if defined(_MISC_SOURCE)

/* macros for struct ifqueue */
#define IF_QFULL(ifq) ((ifq)->ifq_len >= (ifq)->ifq_maxlen)
#define IF_DROP(ifq)  ((ifq)->ifq_drops++)
#if (NEWIFQUEUE)
#define QUEUE_SINGLE_TYPE
#define QUEUE_TYPE struct membuf
#include <zero/queue.h>
#define IF_ENQUEUE(ifq, buf)                                            \
    queueappend(buf, &(ifq)->if_snd.ifq_queue)
#define IF_PREPEND(ifq, buf)                                            \
    queuepush(buf, &(ifq)->if_snd.ifq_queue)
#define IF_DEQUEUE(ifq, buf)                                            \
    ((buf) = queuepop(&(ifq)->if_snd.ifq_queue))
#else /* !NEWIFQUEUE */
#define IF_ENQUEUE(ifq, buf)                                            \
    do {                                                                \
        (buf)->next = NULL;                                             \
        if ((ifq)->tail == NULL) {                                      \
            (ifq)->ifq_head = (buf);                                    \
        } else {                                                        \
            (ifq)->ifq_tail->next = (buf);                              \
        }                                                               \
        (ifq)->ifq_tail = (buf);                                        \
        (ifq)->ifq_len++;                                               \
    } while (0)
#define IF_PREPEND(ifq, buf)                                            \
    do {                                                                \
        (buf)->next = (ifq)->ifq_head;                                  \
        if ((ifq)->ifq_tail == NULL) {                                  \
            (ifq)->ifq_tail = (buf);                                    \
        }                                                               \
        (ifq)->ifq_head = (buf);                                        \
        (ifq)->ifq_len++;                                               \
    } while (0)
#define IF_DEQUEUE(ifq, buf)                                            \
    do {                                                                \
        (buf) = (ifq)->ifq_head;                                        \
        if (buf) {                                                      \
            (ifq)->ifq_head = (buf)->next;                              \
            if ((buf)->next == NULL) {                                  \
                (ifq)->ifq_tail = NULL;                                 \
            }                                                           \
            (buf)->next = NULL;                                         \
            (ifq)->ifq_len--;                                           \
        }                                                               \
    } while (0)
#endif
#define IF_ENQUEUEIF(ifq, buf, ifp)                                     \
    do {                                                                \
        IF_ENQUEUE(ifq, buf);                                           \
        (buf)->ifp = (ifp);                                             \
    } while (0)
#define IF_DEQUEUEIF(ifq, buf, ifp)                                     \
    do {                                                                \
        IF_DEQUEUE(ifq, buf);                                           \
        if (buf) {                                                      \
            (ifp) = (buf)->ifp;                                         \
        }                                                               \
    } while (0)
#define IF_ADJ(buf)                                                     \
    do {                                                                \
        if (!(buf)->len) {                                              \
            struct membuf *next = (buf)->next;                          \
                                                                        \
            memfree(buf);                                               \
            (buf) = next;                                               \
        }                                                               \
    } while (0)
#define IFVERLEN   256
#define IFQ_MAXLEN 128
#define IFQ_SLOWHZ 1
struct ifnet {
    char	        if_version[IFVERLEN];
    char               *if_name;
    short               if_unit;
    short               if_mtu;
    short               if_flags;
    short               if_timer;
    int                 if_metric;
    struct ifaddr      *if_addrlist;
    struct ifqueue {
#if (NEWIFQUEUE)
        struct membuf  *ifq_queue;
#else
        struct membuf  *ifq_head;
        struct membuf  *ifq_tail;
#endif
        long            ifq_len;
        long            ifq_maxlen;
        long            ifq_drops;
    } if_snd;
    int               (*if_init)();
    int               (*if_output)();
    int	              (*if_ioctl)();
    int	              (*if_reset)();
    int	              (*if_watchdog)();
    int	              (*if_start)();
    int	                if_ipackets;
    int	                if_ierrors;
    int	                if_opackets;
    int	                if_oerrors;
    int	                if_collisions;
    int	                d_affinity;
    struct ifnet       *if_next;
    int	                if_type;
    int	                if_sysid_type;
};

#define IFF_UP          (1 << 0)        // interface up
#define IFF_BROADCAST   (1 << 1)        // broadcast address valid
#define IFF_DEBUG       (1 << 2)        // turn debugging on
#define IFF_LOOPBACK    (1 << 3)        // loopback network interface
#define IFF_POINTOPOINT (1 << 4)        // point-to-point link
#define IFF_NOTRAILERS  (1 << 5)        // avoid use of trailers
#define IFF_RUNNING     (1 << 6)        // resources allocated
#define IFF_NOARP       (1 << 7)        // no address resolution protocol
#define IFF_PROMISC     (1 << 8)        // receive ALL packets
#define IFF_ALLMULTI    (1 << 9)        // receive all multicast packets
#define IFF_MASTER      (1 << 10)       // master of a load-balancer
#define IFF_SLAVE       (1 << 11)       // slave of a load-balancer
#define IFF_MULTICAST   (1 << 12)       // supports multicast
#define IFF_PORTSEL     (1 << 13)       // can set media type
#define IFF_AUTOMEDIA   (1 << 14)       // auto media-select active
#define IFF_DYNAMIC     (1 << 15)       // dialup / changing addresses
#define IFF_OACTIVE     (1 << 16)       // transmission in progress
#define IFF_SIMPLEX     (1 << 17)       // can't hear own transmissions

#define ifa_broadaddr ifa_ifu.ifu_broadaddr
#define ifa_dstaddr   ifa_ifu.ifu_dstaddr
struct ifaddr {
    struct sockaddr      ifa_addr;      // interface address
    union {
        struct sockaddr  ifu_broadaddr;
        struct sockaddr  ifu_dstaddr;
    } ifa_ifu;
    struct ifnet        *ifa_ifp;       // back pointer to interface
    struct ifaddr       *ifa_next;      // next address for interface
};

struct ifmap {
    uintptr_t      mem_start;
    uintptr_t      mem_end;
    unsigned short base_addr;
    uint8_t        irq;
    uint8_t        dma;
    uint8_t        port;
    uint8_t        _res[3];
};

#define IFHWADDRLEN      6
#define IFNAMSIZ         IF_NAMESIZE
#define ifr_name	 ifr_ifrn.ifrn_name	/* interface name 	*/
#define ifr_hwaddr	 ifr_ifru.ifru_hwaddr	/* MAC address 		*/
#define ifr_addr	 ifr_ifru.ifru_addr	/* address		*/
#define ifr_dstaddr	 ifr_ifru.ifru_dstaddr	/* other end of p-p lnk	*/
#define ifr_broadaddr	 ifr_ifru.ifru_broadaddr /* broadcast address	*/
#define ifr_netmask	 ifr_ifru.ifru_netmask	/* interface net mask	*/
#define ifr_flags	 ifr_ifru.ifru_flags	/* flags		*/
#define ifr_metric	 ifr_ifru.ifru_ivalue	/* metric		*/
#define ifr_mtu	         ifr_ifru.ifru_mtu	/* mtu			*/
#define ifr_map	         ifr_ifru.ifru_map	/* device map		*/
#define ifr_slave	 ifr_ifru.ifru_slave	/* slave device		*/
#define ifr_data	 ifr_ifru.ifru_data	/* for use by interface	*/
#define ifr_ifindex	 ifr_ifru.ifru_ivalue   /* interface index      */
#define ifr_bandwidth	 ifr_ifru.ifru_ivalue	/* link bandwidth	*/
#define ifr_qlen	 ifr_ifru.ifru_ivalue	/* queue length		*/
#define ifr_newname	 ifr_ifru.ifru_newname	/* New name		*/
#define _IOT_ifreq	 _IOT(_IOTS(char), IFNAMSIZ, _IOTS(char), 16, 0, 0)
#define _IOT_ifreq_short _IOT(_IOTS(char), IFNAMSIZ, _IOTS(short), 1, 0, 0)
#define _IOT_ifreq_int	 _IOT(_IOTS(char), IFNAMSIZ, _IOTS(int), 1, 0, 0)
struct ifreq {
    union {
        char             ifrn_name[IFNAMSIZ];   // e.g. "en0"
    } ifr_ifrn;
    union {
	struct sockaddr  ifru_addr;
	struct sockaddr  ifru_dstaddr;
	struct sockaddr  ifru_broadaddr;
	struct sockaddr  ifru_netmask;
	struct sockaddr  ifru_hwaddr;
	short int        ifru_flags;
	int              ifru_ivalue;
	int              ifru_mtu;
	struct ifmap     ifru_map;
	char             ifru_slave[IFNAMSIZ];  // Just fits the size */
	char             ifru_newname[IFNAMSIZ];
	void           *ifru_data;
    } ifr_ifru;
};

struct ifaliasreq {
    char            ifra_name[IFNAMSIZ];
    struct sockaddr ifra_addr;
    struct sockaddr ifra_broadaddr;
    struct sockaddr ifra_mask;
};

/* used with SIOCGIFCONF for ioctl() */
#define ifc_buf     ifc_ifcu.ifcu_buf
#define ifc_req     ifc_ifcu.ifcu_req
#define _IOT_ifconf _IOT(_IOTS(struct ifconf), 1, 0, 0, 0, 0)
struct ifconf {
    int               ifc_len;
    union {
        void         *ifcu_buf;
        struct ifreq *ifcu_req;
    } ife_ifcu;
};

#endif /* defined(_MISC_SOURCE) */

#endif /* __NET_IF_H__ */

