#ifndef __KERN_MEM_MEMBUF_H__
#define __KERN_MEM_MEMBUF_H__

#include <kern/conf.h>
#include <stdint.h>
#include <stddef.h>
#if (SMP)
#include <mach/asm.h>
#endif
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>
#include <kern/types.h>

/* amount of memory to use for membufs */
#define MEMBUFSMAX          (4 * MEMBUFBLKS)

/* membuf convenience macros */
#define MEMBUF_SIZE         PAGESIZE                    // membuf size
#define MEMBUF_BLK_SHIFT    16                          // blk of 64 K
#define MEMBUF_BLK_SIZE     (1 << MEMBUF_BLK_SHIFT)
#define MEMBUF_DATA_LEN     ((long)(MEMBUF_SIZE - sizeof(struct membufhdr)))
#define MEMBUF_PKT_LEN      ((long)(MEMBUF_DATA_LEN - sizeof(struct mempkthdr)))
#define MEMBUF_BLK_MIN      (MEMBUF_PKT_LEN + 1)
#define MEMBUF_MAX_COMPRESS (MEMBUF_PKT_LEN >> 1)

/* TODO
 * ----
 */
struct membufctl {
    m_ureg_t bufsize;     // MEMBUF_SIZE
    m_ureg_t blkshift;    // MEMBUF_BLK_SHIFT
    m_ureg_t blkmin;      // MEMBUF_BLK_MIN_SIZE
    m_ureg_t datalen;     // MEMBUF_DATA_LEN
    m_ureg_t pktlen;      // MEMBUF_PKT_LEN
    m_ureg_t maxcompress;
};

/* copy parameter */
#define MEMBUF_COPY_ALL     (~0UL)

/* membuf types */
#define MEMBUF_FREE         0  // on free-list
#define MEMBUF_DATA         1  // dynamic allocation
#define MEMBUF_EXT          2  // external storage mapped to membuf
#define MEMBUF_PKTHDR       3  // packet header
#define MEMBUF_SOCK         4  // socket structure
#define MEMBUF_PCB          5  // protocol control block
#define MEMBUF_ROUTE_TAB    6  // routing tables
#define MEMBUF_HOST_TAB     7  // IMP host tables
#define MEMBUF_ADR_TAB      8  // address resolution tables
#define MEMBUF_SONAME       9  // socket name
#define MEMBUF_SOOPTS       10 // socket options
#define MEMBUF_FRAG_TAB     11 // fragment reassembly hedaer
#define MEMBUF_RIGHTS       12 // access rights
#define MEMBUF_IFADDR       13 // interface address
#define MEMBUF_CONTROL      14 // extra-data protocol message
#define MEMBUF_OOBDATA      15 // expedited data
#define MEMBUF_NTYPE        16
#define MEMBUF_NOINIT       (~0L) // for allocating non-initialised membufs */
/* membuf flags */
#define MEMBUF_EXT_BIT       (1 << 0)  // associated external storage
#define MEMBUF_PKTHDR_BIT    (1 << 1)  // start of record
#define MEMBUF_EOR_BIT       (1 << 2)  // end of record
#define MEMBUF_RDONLY_BIT    (1 << 3)  // read-only data
#define MEMBUF_BROADCAST_BIT (1 << 4)  // send/received as link-level broadcast
#define MEMBUF_MULTICAST_BIT (1 << 5)  // send/received as link-level multicast
#define MEMBUF_PROMISC_BIT   (1 << 6)  // packet not for us
#define MEMBUF_VLANTAG_BIT   (1 << 7)  // ethernet vtag is valid
#define MEMBUF_UNUSED_BIT    (1 << 8)  // AVAILABLE */
#define MEMBUF_NOFREE_BIT    (1 << 9)  // membuf in blk, no separate free
#define MEMBUF_PROTO_BIT1    (1 << 10) // protocol-specific flags
#define MEMBUF_PROTO_BIT2    (1 << 11)
#define MEMBUF_PROTO_BIT3    (1 << 12)
#define MEMBUF_PROTO_BIT4    (1 << 13)
#define MEMBUF_PROTO_BIT5    (1 << 14)
#define MEMBUF_PROTO_BIT6    (1 << 15)
#define MEMBUF_PROTO_BIT7    (1 << 16)
#define MEMBUF_PROTO_BIT8    (1 << 17)
#define MEMBUF_PROTO_BIT9    (1 << 18)
#define MEMBUF_PROTO_BIT10   (1 << 19)
#define MEMBUF_PROTO_BIT11   (1 << 20)
#define MEMBUF_PROTO_BIT12   (1 << 21)
/* flags purged when crossing layers */
#define MEMBUF_PROTO_BITS                                               \
    (MEMBUF_PROTO_BIT1 | MEMBUF_PROTO_BIT2 | MEMBUF_PROTO_BIT3          \
     | MEMBUF_PROTO_BIT4 | MEMBUF_PROTO_BIT5 | MEMBUF_PROTO_BIT6        \
     | MEMBUF_PROTO_BIT7 | MEMBUF_PROTO_BIT8 | MEMBUF_PROTO_BIT9        \
     | MEMBUF_PROTO_BIT10 | MEMBUF_PROTO_BIT11 | MEMBUF_PROTO_BIT12)
/* flags copied for pkthdr */
#define MEMBUF_PKT_COPY_BITS                                            \
    (MEMBUF_PKTHDR_BIT | MEMBUF_EOR_BIT | MEMBUF_RDONLY_BIT             \
     | MEMBUF_BROADCAST_BIT | MEMBUF_MULTICAST_BIT | MEMBUF_PROMISC_BIT \
     | MEMBUF_VLANTAG_BIT                                               \
     | MEMBUF_PROTO_BITS)
#define MEMBUF_EXT_COPY_BITS (MEMBUF_EXT_BITS)

/* checksum flg-bits */
#define MEMBUF_CHKSUM_IP        (1 << 0)  // checksum IP
#define MEMBUF_CHKSUM_UDP       (1 << 1)  // checksum UDP
#define MEMBUF_CHKSUM_TCP       (1 << 2)  // checksum TCP
#define MEMBUF_CHKSUM_STCP      (1 << 3)  // checksum UDP
#define MEMBUF_CHKSUM_IP_TSO    (1 << 4)  // TCP segmentation offload
#define MEMBUF_CHKSUM_ISCSI     (1 << 5)  // iSCSI checksum offload
#define MEMBUF_CHKSUM_IP6_UDP   (1 << 9)  // more checksum offloads
#define MEMBUF_CHKSUM_IP6_TCP   (1 << 10)
#define MEMBUF_CHKSUM_IP6_SCTP  (1 << 11)
#define MEMBUF_CHKSUM_IP6_TSO   (1 << 12)
#define MEMBUF_CHKSUM_IP6_ISCSI (1 << 13)
/* hardware-verified inbound checksum support */
#define MEMBUF_CHKSUM_L3_CALC   (1 << 24)
#define MEMBUF_CHKSUM_L3_VALID  (1 << 25)
#define MEMBUF_CHKSUM_L4_CALC   (1 << 26)
#define MEMBUF_CHKSUM_L4_VALID  (1 << 27)
#define MEMBUF_CHKSUM_L5_CALC   (1 << 28)
#define MEMBUF_CHKSUM_L5_VALID  (1 << 29)
#define MEMBUF_CHKSUM_MERGED    (1 << 30)
/* record/packet header in first mb of chain; MEMBUF_PKTHDR is set */
struct mempkthdr {
    struct netif  *recvif;      // receiver interface
    m_ureg_t       size;        // buffer length
    m_ureg_t       len;         // packet length
    uint8_t       *hdr;         // packet header
    int32_t        flg;         // checksum and other flags
    int32_t        chksum;      // checksum data
    struct membuf *aux;         // extra data buffer, e.g. IPSEC
};

/* external buffer types */
#define MEMBUF_EXT_NETDRV     0x01000000 // custom extbuf provide by net driver
#define MEMBUF_EXT_MODULE     0x02000000 // custom module-specific extbuf type
#define MEMBUF_EXT_DISPOSABLE 0x04000000 // may throw away with page-flipping
#define MEMBUF_EXT_EXTREF     0x08000000 // external reference count
#define MEMBUF_EXT_RDONLY     0x10000000 //
#define MEMBUF_EXT_RDWR       0x20000000
#define MEMBUF_EXT_BITS       0xff000000
#define MEMBUF_EXT_BLK        1          // mb block ('cluster')
#define MEMBUF_EXT_FILEBUF    2          // buffer for sendfile()-like things
#define MEMBUF_EXT_PAGE       3          // PAGESIZE-byte buffer
#define MEMBUF_EXT_DUAL_PAGE  4          // 2 * PAGESIZE bytes
#define MEMBUF_EXT_QUAD_PAGE  5          // 4 * PAGESIZE bytes
#define MEMBUF_EXT_PACKET     6          // mb + memblk from packet zone
#define MEMBUF_EXT_DATA       7          // external membuf (M_IOVEC)
struct memexthdr {
    volatile m_atomic_t   nref;
    uint8_t              *data; // buffer base address
    m_ureg_t              size; // buffer size
    m_ureg_t              len;  // buffer data length
    void                (*rel)(void *, void *); // optional custom free()
    void                 *args; // optional argument pointer
    long                  type; // storage type
};

struct membufhdr {
    uint8_t          *data;     // buffer data base address
    m_ureg_t          size;     // buffer size
    m_ureg_t          len;      // # of bytes in membuf
    long              type;     // buffer type
    long              flg;      // flags
    struct membuf    *next;     // next buffer in chain
    struct mempktbuf *nextpkt;  // next packet in chain
};

struct membuf {
    struct membufhdr             hdr;
    union {
        struct {
            struct mempkthdr     pkt;   // MEMBUF_PKTHDR is set
            union {
                struct memexthdr ext;
                uint8_t          data[MEMBUF_PKT_LEN];
            } mem;
        } info;
        uint8_t                  mem[MEMBUF_DATA_LEN];
    } data;
};

#if (SMP)
#define membufincref(buf)       m_atominc(&(buf)->nref)
#define membufdecref(buf)       m_atomdec(&(buf)->nref)
#define membufgetnref(buf, res) m_syncread(&(buf)->nref, (res))
#else
#define membufincref(buf)       ((buf)->nref++)
#define membufdecref(buf)       ((buf)->nref--)
#define membufgetref(buf, res)  ((buf)->nref)
#endif

#define membufdata(buf)         ((buf)->data.mem)
#define membufsize(buf)         ((buf)->hdr.size)
#define membuflen(buf)          ((buf)->hdr.len)
#define membuftype(buf)         ((buf)->hdr.type)
#define membufflg(buf)          ((buf)->hdr.flg)
#define membufnext(buf)         ((buf)->hdr.next)
#define membufpkthdr(buf)       (((buf)->data.info.pkt))
#define membufpktdata(buf)      ((buf)->data.info.mem.data)
#define membufpktsize(buf)      ((buf)->data.info.pkt.size)
#define membufpktlen(buf)       ((buf)->data.info.pkt.len)
#define membufexthdr(buf)       (((buf)->data.info.mem.ext))
#define membufextdata(buf)      (((buf)->data.info.mem.data))
#define membufextsize(buf)      ((buf)->data.info.mem.ext.size)
#define membufextlen(buf)       ((buf)->data.info.mem.ext.len)

struct membufauxpkt {
    long af;
    long type;
};

struct membufpool {
    m_atomic_t     lk;
    long           flg;
    m_ureg_t       nbuf;
    m_ureg_t       nblk;
    struct membuf *buflist;
    uint8_t         _pad[CLSIZE
                         - sizeof(m_atomic_t)
                         - sizeof(long)
                         - 2 * sizeof(m_ureg_t)
                         - sizeof(struct membuf *)];
};

#include <kern/mem/bits/membuf.h>

#endif /* __KERN_MEM_MEMBUF_H__ */

