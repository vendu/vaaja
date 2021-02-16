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
#define ZEN_MEM_ZONE_BITS       2
#define ZEN_MEM_ZONE_MASK       0x03
#define zeninitmempool(mp)      ((mp)->head = (mp)->bmap,               \
                                 (mp)->tail = (mp)->bmap)
struct zenmempool {
    void                      (*free)(void *);
    void                       *head;
    void                       *tail;
    m_word_t                    zone;
    m_word_t                    flg;
    m_word_t                    slot;
    m_word_t                    nitem;
    volatile m_atomic_t         nref;
    //    m_word_t                    bmap[ZEN_MAX_POOL_ITEMS / CHAR_BIT];
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
#define ZEN_MEM_MAX_BLK         (MACH_PAGE_SIZE << 8)
#define ZEN_MEM_SLAB_BLKS       (ZEN_MEM_BLK_SLAB_SIZE / ZEN_MEM_MIN_BLK)
#define ZEN_MEM_BLK_SLAB_SIZE   (8 * MACH_PAGE_SIZE)
/* runs; multiple-of-page allocations */
#define ZEN_MEM_MIN_RUN         MACH_PAGE_SIZE
#define ZEN_MEM_MAX_RUN         (ZEN_MEM_RUN_SLOTS * MACH_PAGE_SIZE)
#define ZEN_MEM_RUN_SLOTS       8
#define ZEN_MEM_RUN_SLAB_SIZE   (ZEN_MEM_RUN_SLOTS * MACH_PAGE_SIZE)
#endif

struct zenmembuf {
    uintptr_t                   adr;
    volatile m_atomic_t         nref;
    m_size_t                    size;
    m_word_t                    flg;
};

struct zenmemslab {
    struct zenmembuf            bufhdr;
    struct zenmemqueue         *queue;
    struct zenmemslab          *prev;
    struct zenmemslab          *next;
    m_word_t                    type;   // BLK, RUN, BIG, BUF, DMA (0-4)
    uint8_t                     bmap[ZEN_MAX_SLAB_ITEMS / CHAR_BIT];
};

#endif /* ZEN_MEM_H */

