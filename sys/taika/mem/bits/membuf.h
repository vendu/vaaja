#ifndef __KERN_MEM_BITS_MEMBUF_H__
#define __KERN_MEM_BITS_MEMBUF_H__

/*
 * REFERENCES
 * ----------
 * https://people.freebsd.org/~bmilekic/code/mb_slab/mbuf.h
 * https://people.freebsd.org/~bmilekic/code/mb_slab/uipc_mbuf.c
 */

#include <stddef.h>
#include <stdint.h>
#include <mach/asm.h>
#include <zero/trix.h>
#include <kern/malloc.h>
#include <kern/util.h>

/* TODO: perhaps hack a dedicated pool/allocator for memory blocks? */

/* check if it's safe to write to buffer's data region */
#define membufwritable(buf)                                             \
    (!((buf)->hdr.flg & MEMBUF_RDONLY_BIT)                              \
     && (!(membufexthdr(buf)->flg & MEMBUF_EXT_RDONLY)                  \
         || (membufexthdr(buf)->nref > 1)))
/* longword-align buffer data pointer for object of len */
#define membufaligndata(buf, len)                                       \
    ((buf)->hdr.data += ((uintptr_t)&membufdata(buf) + MEMBUF_DATA_LEN  \
                         - (len)) & ~(sizeof(uintptr_t) - 1))
/* longword-align buffer packet data pointer for object of len */
#define membufalignpkt(buf, len)                                        \
    ((buf)->hdr.data = (void *)(((uintptr_t)&membufpktdata(buf)         \
                                 + (MEMBUF_PKT_LEN - (len)))            \
                                & ~(sizeof(uintptr_t) - 1)))
/* compute amount of space before current start of data */
#define membufleadspace(buf)                                            \
    (((buf)->hdr.flg & MEMBUF_EXT_BIT)                                  \
     ? 0                                                                \
     : (((buf)->hdr.flg & MEMBUF_PKTHDR)                                \
        ? ((buf)->hdr.data - membufpktdata(buf))                        \
        : ((buf)->hdr.data - membufdata(buf))))
/* compute amount of space after current end of data */
#define membuftrailspace(buf)                                           \
    (((buf)->hdr.flg & MEMBUF_EXT_BIT)                                  \
     ? (membufextsize(buf) - membufextlen(buf))                         \
     : (((buf)->hdr.flg & MEMBUF_PKTHDR)                                \
        ? (membufpktsize(buf) - membufpktlen(buf))                      \
        : (membufsize(buf) - membuflen(buf))))

#endif /* __KERN_MEM_BITS_MEMBUF_H__ */

