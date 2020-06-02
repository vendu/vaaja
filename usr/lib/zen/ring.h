#ifndef __ZERO_RING_H__
#define __ZERO_RING_H__

/* FIXME: do this! :) */
#define RINGATOMIC 0

/* RING_MALLOC  - function used to allocate data buffer */
/* RING_FREE    - function used to free buffers */
/* RING_MEMCPY  - function used to copy data */
/* RING_ITEM    - type of items in ring buffer */
/* RING_INVAL   - invalid/non-present item value */

#if !defined(RINGSHAREBUF)
#define RINGSHAREBUF 0
#endif

#include <features.h>
#include <stdint.h>
#include <limits.h>
#if defined(_ZERO_SOURCE) && (RINGSHAREBUF)
#include <sys/syscall.h>
#include <sys/zero/syscall.h>
#include <kern/mem/obj.h>
#endif
#include <zero/cdefs.h>
#include <mach/param.h>
#if (!RINGATOMIC)
#include <mt/mtx.h>
#endif
#include <zero/trix.h>
#if !defined(RING_MALLOC) || !defined(RING_FREE)
#undef RING_MALLOC
#undef RING_FREE
#if defined(__KERNEL__)
#include <kern/malloc.h>
#define RING_MALLOC(sz)            kmalloc(sz)
#define RING_FREE(ptr)             kfree(ptr)
#else
#include <stdlib.h>
#define RING_MALLOC(sz)            malloc(sz)
#define RING_FREE(ptr)             free(ptr)
#endif
#endif
#if !defined(RING_MEMCPY)
#include <string.h>
#define RING_MEMCPY(dest, src, nb) memcpy(dest, src, nb)
#endif

#if (RINGSHAREBUF) && !defined(__KERNEL__)
#if defined(_ISOC11_SOURCE) && (_ISOC11_SOURCE)
#defined RING_VALLOC(n)           aligned_alloc(PAGESIZE, n)
#elif (((defined(_BSD_SOURCE) && (_BSD_SOURCE))                         \
        || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500             \
                                       || ((defined(_XOPEN_SOURCE_EXTENDED) \
                                            && (_XOPEN_SOURCE_EXTENDED)))))) \
       && !((USEPOSIX200112)                                            \
            || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600))))     \
#define RING_VALLOC(n)            valloc(n)
#elif (USEPOSIX200112 || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600)))
static __inline__ void *
RING_VALLOC(size_t n)
{
    void *ptr;

    if (posix_memalign(&ptr, PAGESIZE, n)) {

        return NULL;
    }

    return ptr;
}
#else
#define RING_VALLOC(n) memalign(PAGESIZE, n)
#endif
#endif /* RINGSHAREBUF && !__KERNEL__ */

/* flg-member bits */
#define RINGBUF_INIT (1 << 0)
struct ringbuf {
    zerofmtx   lk;
    long       flg;
    long       nitem;
    RING_ITEM *base;
    RING_ITEM *lim;
    RING_ITEM *inptr;
    RING_ITEM *outptr;
    long       pad;
    /* data buffer */
#if (RINGSHAREBUF)
    uint8_t   *data;
#else
    uint8_t    data[VLA];
#endif
} ALIGNED(PAGESIZE);

/*
 * initialise ring buffer
 * - if base == NULL, allocate the data buffer
 */
static __inline__ long
ringinit(void *ptr, void *base, long nitem)
{
    struct ringbuf *buf = ptr;
    long            retval = 0;

    fmtxlk(&buf->lk);
    if (buf->flg & RINGBUF_INIT) {
        fmtxunlk(&buf->lk);

        return 1;
    }
    if (!base) {
#if (RINGSHAREBUF)
        base = RING_VALLOC(nitem * sizeof(RING_ITEM));
#elif defined(RING_MALLOC)
        base = RING_MALLOC(nitem * sizeof(RING_ITEM));
#else
        base = &buf->data;
#endif
        if (base) {
            retval++;
        }
    } else {
        retval++;
    }
    if (base) {
        buf->flg |= RINGBUF_INIT;
        buf->nitem = nitem;
        buf->base = base;
        buf->lim = (RING_ITEM *)((uint8_t *)base + nitem * sizeof(RING_ITEM));
        buf->inptr = base;
        buf->outptr = base;
    }
    fmtxunlk(&buf->lk);

    return retval;
}

#if defined(_ZERO_SOURCE) && (RINGSHAREBUF)

static __inline__ long
ringmapbuf(struct ringbuf *buf, long flg, struct perm *perm)
{
    struct memreg mem;

    mem.perm = perm;
    mem.flg = MEM_UMAP | flg;
    mem.adr = buf->base;
    mem.ofs = 0;
    mem.len = buf->nitem * sizeof(RING_ITEM);
//    syscall(SYS_MCTL, MEM_SHMAP, MEM_DUALMAP, &mem);
}

#endif /* defined(_ZERO_SOURCE) && (RINGSHAREBUF) */

/* fetch next item from ring buffer */
static __inline__ RING_ITEM
ringget(struct ringbuf *buf)
{
    RING_ITEM item = RING_INVAL;

    fmtxlk(&buf->lk);
    if (buf->outptr == buf->lim) {
        buf->outptr = buf->base;
    }
    if (buf->outptr != buf->inptr) {
        item = *buf->outptr++;
    }
    fmtxunlk(&buf->lk);

    return item;
}

/* queue item into ring buffer */
static __inline__ RING_ITEM
ringput(struct ringbuf *buf, RING_ITEM val)
{
    RING_ITEM item = RING_INVAL;

    fmtxlk(&buf->lk);
    if (buf->inptr == buf->lim) {
        buf->inptr = buf->base;
    }
    if (buf->inptr != buf->outptr) {
        *buf->inptr++ = val;
        item = val;
    } else if (!(buf->flg & RINGBUF_INIT)) {
        *buf->inptr++ = val;
        item = val;
        buf->flg |= RINGBUF_INIT;
    }
    fmtxunlk(&buf->lk);

    return item;
}

/* fetch next items from ring buffer */
static __inline__ long
ringgetmany(struct ringbuf *buf, RING_ITEM *dest, long len)
{
    long nitem;
    long ret;

    fmtxlk(&buf->lk);
    if (buf->outptr > buf->inptr) {
        nitem = buf->lim - buf->outptr;
    } else {
        nitem = buf->inptr - buf->outptr;
    }
    nitem = min(len, nitem);
    len -= nitem;
    ret = nitem;
    while (nitem--) {
        *dest++ = *buf->outptr++;
    }
    if (buf->outptr == buf->lim) {
        buf->outptr = buf->base;
    }
    if (buf->outptr != buf->inptr) {
        nitem = buf->inptr - buf->outptr;
        nitem = min(len, nitem);
        ret += nitem;
        while (nitem--) {
            *dest++ = *buf->outptr++;
        }
    }
    fmtxunlk(&buf->lk);

    return ret;
}

/* queue items into ring buffer */
static __inline__ long
ringputmany(struct ringbuf *buf, RING_ITEM *src, long len)
{
    long nitem;
    long ret;

    fmtxlk(&buf->lk);
    if (buf->inptr < buf->outptr) {
        nitem = buf->outptr - buf->inptr;
    } else {
        nitem = buf->lim - buf->inptr;
    }
    nitem = min(len, nitem);
    len -= nitem;
    ret = nitem;
    while (nitem--) {
        *buf->inptr++ = *src++;
    }
    if (buf->inptr == buf->lim) {
        buf->inptr = buf->base;
    }
    if (buf->inptr != buf->outptr) {
        nitem = buf->outptr - buf->inptr;
        nitem = min(len, nitem);
        ret += nitem;
        while (nitem--) {
            *buf->inptr++ = *src++;
        }
    }
    fmtxunlk(&buf->lk);

    return ret;
}

/* fetch next items from ring buffer */
static __inline__ long
ringgetmany32(struct ringbuf *buf, int32_t *dest, long len)
{
    int32_t *ptr;
    int8_t  *i8ptr;
    long     nitem;
    long     ret;
    long     ofs;

    fmtxlk(&buf->lk);
    i8ptr = (int8_t *)buf->outptr;
    ofs = sizeof(int32_t) - ((uintptr_t)i8ptr & (sizeof(int32_t) - 1));
    i8ptr += ofs;
    ptr = (int32_t *)i8ptr;
    if ((RING_ITEM *)ptr < buf->inptr) {
        nitem = (int8_t *)buf->inptr - (int8_t *)ptr;
    } else {
        nitem = (int8_t *)buf->lim - (int8_t *)ptr;
    }
    nitem = min(len, nitem >> 2);
    len -= nitem;
    ret = nitem;
    while (nitem--) {
        *dest++ = *ptr++;
    }
    if ((RING_ITEM *)ptr == buf->lim) {
        ptr = (int32_t *)buf->base;
    }
    if ((RING_ITEM *)ptr != buf->inptr) {
        nitem = (int8_t *)buf->inptr - (int8_t *)ptr;
        nitem = min(len, nitem >> 2);
        ret += nitem;
        while (nitem--) {
            *dest++ = *ptr++;
        }
    }
    buf->outptr = (RING_ITEM *)ptr;
    fmtxunlk(&buf->lk);

    return ret;
}

/* queue items into ring buffer */
static __inline__ long
ringputmany32(struct ringbuf *buf, int32_t *src, long len)
{
    int32_t *ptr;
    int8_t  *i8ptr;
    long     nitem;
    long     ret;
    long     ofs;

    fmtxlk(&buf->lk);
    i8ptr = (int8_t *)buf->inptr;
    ofs = sizeof(int32_t) - ((uintptr_t)i8ptr & (sizeof(int32_t) - 1));
    ptr = (int32_t *)(i8ptr + ofs);
#if defined(RING_PAD_BYTE)
    while (ofs--) {
        *i8ptr++ = RING_PAD_BYTE;
    }
#endif
    if ((RING_ITEM *)ptr < buf->outptr) {
        nitem = (int8_t *)buf->outptr - (int8_t *)ptr;
    } else {
        nitem = (int8_t *)buf->lim - (int8_t *)ptr;
    }
    nitem = min(len, nitem >> 2);
    len -= nitem;
    ret = nitem;
    while (nitem--) {
        *ptr++ = *src++;
    }
    if ((RING_ITEM *)ptr == buf->lim) {
        ptr = (int32_t *)buf->base;
    }
    if ((RING_ITEM *)ptr != buf->outptr) {
        nitem = (int8_t *)buf->outptr - (int8_t *)ptr;
        nitem = min(len, nitem >> 2);
        ret += nitem;
        while (nitem--) {
            *ptr++ = *src++;
        }
    }
    buf->inptr = (RING_ITEM *)ptr;
    fmtxunlk(&buf->lk);

    return ret;
}

#if defined(RING_MALLOC)

/* resize data buffer */
static __inline__ void *
ringresize(struct ringbuf *buf, long nitem)
{
    RING_ITEM *ptr = NULL;
    RING_ITEM *src;
    long       nin;
    long       nout;

    fmtxlk(&buf->lk);
    if (nitem > buf->nitem) {
        nin = (int8_t *)buf->inptr - (int8_t *)buf->base;
        nout = (int8_t *)buf->outptr - (int8_t *)buf->base;
        src = buf->base;
        ptr = RING_MALLOC(nitem * sizeof(RING_ITEM));
        if (ptr) {
            if (src && (buf->flg & RINGBUF_INIT)) {
                RING_MEMCPY(ptr, src, buf->nitem);
            }
            RING_FREE(src);
            buf->nitem = nitem;
            buf->base = ptr;
            buf->lim = (RING_ITEM *)((int8_t *)ptr
                                     + nitem * sizeof(RING_ITEM));
            buf->inptr = (RING_ITEM *)((int8_t *)ptr + nin);
            buf->outptr = (RING_ITEM *)((int8_t *)ptr + nout);
        }
    } else {
        ptr = buf->base;
    }
    fmtxunlk(&buf->lk);

    return ptr;
}

#endif /* RING_MALLOC */

#endif /* __ZERO_RING_H__ */

