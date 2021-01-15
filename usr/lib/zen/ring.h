/* FIXME: do these! :) */
#undef ZEN_RINGATOMIC
#undef ZEN_RINGLOCKFREE

/* ZEN_RING_MALLOC  - function used to allocate data buffer */
/* ZEN_RING_FREE    - function used to free buffers */
/* ZEN_RING_MEMCPY  - function used to copy data */
/* ZEN_RING_ITEM    - type of items in ring buffer */
/* ZEN_RING_INVAL   - invalid/non-present item value */

#if !defined(ZEN_RINGSHAREBUF)
#define ZEN_RINGSHAREBUF 0
#endif

#include <features.h>
#include <stdint.h>
#include <limits.h>
/* #if defined(_ZERO_SOURCE) && (ZEN_RINGSHAREBUF) */
/* #include <sys/syscall.h> */
/* #include <sys/zero/syscall.h> */
/* #include <kern/mem/obj.h> */
/* #endif */
#include <env/cdefs.h>
#include <mach/param.h>
#include <mach/asm.h>

#if !defined(ZEN_RINGATOMIC) && !defined(ZEN_RINGLOCKFREE)

#include <mt/mtx.h>
#define zenlkring(rb)                   mtlkfmtx(&rb->lk)
#define zenunlkring(rb)                 mtunlkfmtx(&rb->lk)
#define zenlkringinp(rb)
#define zenunlkringinp(rb, inp)
#define zenlkringoutp(rb)
#define zenunlkringoutp(rb, outp)

#elif defined(ZEN_RINGLOCKFREE)

#define zenlkring(rb)
#define zenunlkring(rb)
#define zenlkringinp(rb)
#define zenunlkringinp(rb, inp)
#define zenlkringoutp(rb)
#define zenunlkringoutp(rb, outp)

#else /* defined(ZEN_RINGATOMIC) */

#define zenlkring(rb)
#define zenunlkring(rb)

static C_INLINE void
zenlkringinp(struct ringbuf *rb)
{
    do {
        while ((uintptr_t)rb->inp & MT_ADR_LK_BIT) {
            m_waitspin();
        }
        if (mttrylkbit(&rb->inptr, MT_ADR_LK_BIT_POS)) {

            break;
        }
    } while (1);
}

static C_INLINE void
zenunlkringinp(struct ringbuf *rb, RING_ITEM val)
{
    m_atomwrite(&rb->inptr, val);
}

static C_INLINE void
zenlkringoutp(struct ringbuf *rb)
{
    do {
        while ((uintptr_t)rb->outp & MT_ADR_LK_BIT) {
            m_waitspin();
        }
        if (mttrylkbit(&rb->outptr, MT_ADR_LK_BIT_POS)) {

            break;
        }
    } while (1);
}

static C_INLINE void
zenunlkringoutp(struct ringbuf *rb, RING_ITEM val)
{
    m_atomwrite(&rb->outptr, val);
}

#endif

#include <env/trix.h>

#if defined(__kernel__)
#undef ZEN_RING_MALLOC
#undef ZEN_RING_FREE
#undef ZEN_RING_MEMCPY
#include <sys/zen/mem.h>
#include <sys/zen/util.h>
#define ZEN_RING_MALLOC(sz)             kmalloc(sz)
#define ZEN_RING_FREE(ptr)              kfree(ptr)
#define ZEN_RING_MEMCPY(dest, src, nb)  kmemcpy(dest, src, nb)
#else /* !defined(__kernel__) */
#include <stdlib.h>
#define ZEN_RING_MALLOC(sz)             malloc(sz)
#define ZEN_RING_FREE(ptr)              free(ptr)
#endif
#if !defined(ZEN_RING_MEMCPY)
#if defined(ZEN_RING_BCOPY)
#include <strings.h>
#define ZEN_RING_MEMCPY(dest, src, nb)  bcopy(src, dest, nb)
#else /* !defined(ZEN_RING_BCOPY) */
#include <string.h>
#define ZEN_RING_MEMCPY(dest, src, nb)  memcpy(dest, src, nb)
#endif /* defined(ZEN_RING_BCOPY) */
#endif /* !defined(ZEN_RING_MEMCPY) */

#endif /* defined(__kernel__) */

#if (ZEN_RINGSHAREBUF) && !defined(__kernel__)

#if defined(_ISOC11_SOURCE) && (_ISOC11_SOURCE)
#defined ZEN_RING_VALLOC(n)           aligned_alloc(MACH_PAGE_SIZE, n)
#elif (((defined(_BSD_SOURCE) && (_BSD_SOURCE))                         \
        || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500             \
                                       || ((defined(_XOPEN_SOURCE_EXTENDED) \
                                            && (_XOPEN_SOURCE_EXTENDED)))))) \
       && !((USEPOSIX200112)                                            \
            || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600))))     \
#define ZEN_RING_VALLOC(n)            valloc(n)
#elif (USEPOSIX200112 || (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600)))
static __inline__ void *
ZEN_RING_VALLOC(size_t n)
{
    void *ptr;

    if (posix_memalign(&ptr, MACH_PAGE_SIZE, n)) {

        return NULL;
    }

    return ptr;
}
#else
#define ZEN_RING_VALLOC(n) memalign(MACH_PAGE_SIZE, n)
#endif

#endif /* ZEN_RINGSHAREBUF && !__kernel__ */

/* flg-member bits */
#define ZEN_RINGBUF_INIT (1 << 0)
struct ringbuf {
    mtfmtx          lk;
    long            flg;
    long            nitem;
    ZEN_RING_ITEM  *base;
    ZEN_RING_ITEM  *lim;
    ZEN_RING_ITEM  *inptr;
    ZEN_RING_ITEM  *outptr;
    long            pad;
    /* data buffer */
#if (ZEN_RINGSHAREBUF)
    uint8_t        *data;
#else
    uint8_t         data[C_VLA];
#endif
} C_ALIGNED(MACH_PAGE_SIZE);

/*
 * initialise ring buffer
 * - if base == NULL, allocate the data buffer
 */
static __inline__ long
ringinit(void *ptr, void *base, long nitem)
{
    struct ringbuf *buf = ptr;
    long            retval = 0;

    mtlkfmtx(&buf->lk);
    if (buf->flg & ZEN_RINGBUF_INIT) {
        mtunlkfmtx(&buf->lk);

        return 1;
    }
    if (!base) {
#if (ZEN_RINGSHAREBUF)
        base = ZEN_RING_VALLOC(nitem * sizeof(ZEN_RING_ITEM));
#elif defined(ZEN_RING_MALLOC)
        base = ZEN_RING_MALLOC(nitem * sizeof(ZEN_RING_ITEM));
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
        buf->flg |= ZEN_RINGBUF_INIT;
        buf->nitem = nitem;
        buf->base = base;
        buf->lim = (ZEN_RING_ITEM *)((uint8_t *)base + nitem * sizeof(ZEN_RING_ITEM));
        buf->inptr = base;
        buf->outptr = base;
    }
    mtunlkfmtx(&buf->lk);

    return retval;
}

#if defined(_ZERO_SOURCE) && (ZEN_RINGSHAREBUF)

static __inline__ long
ringmapbuf(struct ringbuf *buf, long flg, struct perm *perm)
{
    struct memreg mem;

    mem.perm = perm;
    mem.flg = MEM_UMAP | flg;
    mem.adr = buf->base;
    mem.ofs = 0;
    mem.len = buf->nitem * sizeof(ZEN_RING_ITEM);
//    syscall(SYS_MCTL, MEM_SHMAP, MEM_DUALMAP, &mem);
}

#endif /* defined(_ZERO_SOURCE) && (ZEN_RINGSHAREBUF) */

/* fetch next item from ring buffer */
static __inline__ ZEN_RING_ITEM
ringget(struct ringbuf *buf)
{
    ZEN_RING_ITEM item = ZEN_RING_INVAL;

    mtlkfmtx(&buf->lk);
    if (buf->outptr == buf->lim) {
        buf->outptr = buf->base;
    }
    if (buf->outptr != buf->inptr) {
        item = *buf->outptr++;
    }
    mtunlkfmtx(&buf->lk);

    return item;
}

/* queue item into ring buffer */
static __inline__ ZEN_RING_ITEM
ringput(struct ringbuf *buf, ZEN_RING_ITEM val)
{
    ZEN_RING_ITEM item = ZEN_RING_INVAL;

    mtlkfmtx(&buf->lk);
    if (buf->inptr == buf->lim) {
        buf->inptr = buf->base;
    }
    if (buf->inptr != buf->outptr) {
        *buf->inptr++ = val;
        item = val;
    } else if (!(buf->flg & ZEN_RINGBUF_INIT)) {
        *buf->inptr++ = val;
        item = val;
        buf->flg |= ZEN_RINGBUF_INIT;
    }
    mtunlkfmtx(&buf->lk);

    return item;
}

/* fetch next items from ring buffer */
static __inline__ long
ringgetmany(struct ringbuf *buf, ZEN_RING_ITEM *dest, long len)
{
    long nitem;
    long ret;

    mtlkfmtx(&buf->lk);
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
    mtunlkfmtx(&buf->lk);

    return ret;
}

/* queue items into ring buffer */
static __inline__ long
ringputmany(struct ringbuf *buf, ZEN_RING_ITEM *src, long len)
{
    long nitem;
    long ret;

    mtlkfmtx(&buf->lk);
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
    mtunlkfmtx(&buf->lk);

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

    mtlkfmtx(&buf->lk);
    i8ptr = (int8_t *)buf->outptr;
    ofs = sizeof(int32_t) - ((uintptr_t)i8ptr & (sizeof(int32_t) - 1));
    i8ptr += ofs;
    ptr = (int32_t *)i8ptr;
    if ((ZEN_RING_ITEM *)ptr < buf->inptr) {
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
    if ((ZEN_RING_ITEM *)ptr == buf->lim) {
        ptr = (int32_t *)buf->base;
    }
    if ((ZEN_RING_ITEM *)ptr != buf->inptr) {
        nitem = (int8_t *)buf->inptr - (int8_t *)ptr;
        nitem = min(len, nitem >> 2);
        ret += nitem;
        while (nitem--) {
            *dest++ = *ptr++;
        }
    }
    buf->outptr = (ZEN_RING_ITEM *)ptr;
    mtunlkfmtx(&buf->lk);

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

    mtlkfmtx(&buf->lk);
    i8ptr = (int8_t *)buf->inptr;
    ofs = sizeof(int32_t) - ((uintptr_t)i8ptr & (sizeof(int32_t) - 1));
    ptr = (int32_t *)(i8ptr + ofs);
#if defined(ZEN_RING_PAD_BYTE)
    while (ofs--) {
        *i8ptr++ = ZEN_RING_PAD_BYTE;
    }
#endif
    if ((ZEN_RING_ITEM *)ptr < buf->outptr) {
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
    if ((ZEN_RING_ITEM *)ptr == buf->lim) {
        ptr = (int32_t *)buf->base;
    }
    if ((ZEN_RING_ITEM *)ptr != buf->outptr) {
        nitem = (int8_t *)buf->outptr - (int8_t *)ptr;
        nitem = min(len, nitem >> 2);
        ret += nitem;
        while (nitem--) {
            *ptr++ = *src++;
        }
    }
    buf->inptr = (ZEN_RING_ITEM *)ptr;
    mtunlkfmtx(&buf->lk);

    return ret;
}

#if defined(ZEN_RING_MALLOC)

/* resize data buffer */
static __inline__ void *
ringresize(struct ringbuf *buf, long nitem)
{
    ZEN_RING_ITEM *ptr = NULL;
    ZEN_RING_ITEM *src;
    long       nin;
    long       nout;

    mtlkfmtx(&buf->lk);
    if (nitem > buf->nitem) {
        nin = (int8_t *)buf->inptr - (int8_t *)buf->base;
        nout = (int8_t *)buf->outptr - (int8_t *)buf->base;
        src = buf->base;
        ptr = ZEN_RING_MALLOC(nitem * sizeof(ZEN_RING_ITEM));
        if (ptr) {
            if (src && (buf->flg & ZEN_RINGBUF_INIT)) {
                ZEN_RING_MEMCPY(ptr, src, buf->nitem);
            }
            ZEN_RING_FREE(src);
            buf->nitem = nitem;
            buf->base = ptr;
            buf->lim = (ZEN_RING_ITEM *)((int8_t *)ptr
                                     + nitem * sizeof(ZEN_RING_ITEM));
            buf->inptr = (ZEN_RING_ITEM *)((int8_t *)ptr + nin);
            buf->outptr = (ZEN_RING_ITEM *)((int8_t *)ptr + nout);
        }
    } else {
        ptr = buf->base;
    }
    mtunlkfmtx(&buf->lk);

    return ptr;
}

#endif /* ZEN_RING_MALLOC */

