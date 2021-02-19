#ifndef ZEN_MEM_H
#define ZEN_MEM_H

#include <mach/param.h>
#include <mach/types.h>

#define ZEN_ADR_LK_BIT_POS      0
#define ZEN_MEM_BLK             0
#define ZEN_MEM_RUN             1
#define ZEN_MEM_BIG             2

#if (MACH_PTR_SIZE == 4)
#define ZEN_MEM_MAX_SLABS       129     // 4 gigabytes
#elif (MACH_PTR_SIZE == 4)
#define ZEN_MEM_MAX_SLABS       16384   // 1 terabyte
#endif

struct zenmemconf {
    m_word_t                    linesize;
    m_word_t                    stridesize;
    m_word_t                    pagesize;
    m_word_t                    hugesize;
};

#define ZEN_MEM_ZONE_DMA        0
#define ZEN_MEM_ZONE_BUF        1
#define ZEN_MEM_ZONE_HEAP       2
#define ZEN_MEM_ZONE_BITS       (ZEN_MEM_ZONE_HEAP | ZEN_MEM_ZONE_BUF)
#define ZEN_MEM_ZONE_MASK       0x03
#define ZEN_MAX_POOL_ITEMS      8
struct zenmempool {
    void                      (*free)(void *);
    void                       *head;
    void                       *tail;
    volatile m_atomic_t         nref;
    m_word_t                    zone;
    m_word_t                    flg;
    m_word_t                    slot;
    m_word_t                    nitem;
    m_byte_t                    bmap[ZEN_MAX_POOL_ITEMS / CHAR_BIT];
};

/*
 * NOTES
 * -----
 * x86      -> cl = 32, blks = 256, page = 4096, slab = 8192
 * x86-64   -> cl = 64, blks = 512, page = 8192, slab = 32768
 * - ZEN_MEM_RUN_SLOTS must be <= 4/3 (32/64-bit) so the max size for
 * fastudiv16 is 65536
 */
/* blocks; sub-page [cacheline] allocations */
#define ZEN_MEM_MIN_BLK         MACH_CL_SIZE
#define ZEN_MEM_MAX_BLK         (MACH_PAGE_SIZE >> 1)
#define ZEN_MEM_SLAB_BLKS       (ZEN_MEM_BLK_SLAB_SIZE / ZEN_MEM_MIN_BLK)
#define ZEN_MEM_BLK_SLAB_SIZE   (8 * MACH_PAGE_SIZE)
/* runs; multiple-of-page allocations */
#define ZEN_MEM_MIN_RUN         MACH_PAGE_SIZE
#define ZEN_MEM_MAX_RUN         (ZEN_MEM_RUN_SLOTS * MACH_PAGE_SIZE)
#define ZEN_MEM_RUN_SLOTS       8
#define ZEN_MEM_RUN_SLAB_SIZE   (ZEN_MEM_RUN_SLOTS * MACH_PAGE_SIZE)

#define ZEN_MAX_SLAB_ITEMS      ZEN_MEM_SLAB_BLKS

struct zenmembuf {
    m_atomic_t                  hazptr[2];
    m_adr_t                     adr;
    volatile m_atomic_t         nref;
    m_size_t                    size;
    m_word_t                    flg;
};

struct zenmemdeque {
    struct zenmemdeque         *self;
    void                       *prev;
    void                       *next;
};

struct zenmemslabinfo {
    m_word_t                    type;
    m_word_t                    slot;
    m_byte_t                    bmap[ZEN_MAX_SLAB_ITEMS / CHAR_BIT];
}

struct zenmemslab {
    struct zenmembuf            bufhdr;
    struct zenmemdeque          deqhdr;
    struct zenmemslabinfo       slabhdr;
};

C_STATIC C_INLINE
void *zencoloradr(void *ptr, long type, long size, long blksize)
{
    void                      **pptr;
    m_adr_t                     adr = (m_adr_t)ptr;
    m_word_t                    cmax = blksize - size;
    m_word_t                    rnd = zenurand();
    m_word_t                    ndx;
    m_word_t                    ofs;

    cmax >>= 1;
    if (type == ZEN_MEM_BLK) {
        ndx = modu12(rnd);
        ofs = memprimetab[ndx];
        ofs = roundup2(ofs, sizeof(double));
        adr += ofs;
        pptr = (void **)adr;
        ofs >>= MACH_CL_SIZE_LOG2;
        pptr[-1] = ptr;
        (m_word_t *)pptr[-2] = ofs;
    } else if (type == ZEN_MEM_RUN) {
        /* TODO */
    }
}

#endif /* ZEN_MEM_H */

