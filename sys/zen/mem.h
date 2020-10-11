#ifndef __SYS_ZEN_MEM_H__
#define __SYS_ZEN_MEM_H__

#include <limits.h>
#include <stddef.h>
#include <zero/cdefs.h>
//#include <mach/types.h>
//#include <mach/atomic.h>
#include <sys/zen/util.h>

#define ZEN_ADR_LK_BIT_POS  0

#define ZEN_MEM_BLK         0
#define ZEN_MEM_RUN         1
#define ZEN_MEM_BIG         2

static C_INLINE void *
kmalloc(m_size_t size)
{
    kpanic("kmalloc() not implemented yet", SIGSYS);

    return NULL;
}

struct zenmemconf {
    m_word_t                    pagesize;
    m_word_t                    hugesize;
    m_word_t                    clsize;
    m_word_t                    strsize;
};

#define ZEN_MEM_ZONE_DMA        0
#define ZEN_MEM_ZONE_BUF        1
#define ZEN_MEM_ZONE_HEAP       2
#define ZEN_MEM_ZONE_BITS       2
#define ZEN_MEM_POOL_ZONE_MASK  0x03
#define zeninitmempool(mp)      ((mp)->head = &(mp)->flg,               \
                                 (mp)->tail = &(mp)->flg)
struct zenmempool {
    void                      (*free)(void *);
    void                       *head;
    void                       *tail;
    m_word_t                    dummy;
    m_word_t                    type;
    m_word_t                    flg;
    m_word_t                    slot;
    m_word_t                    nitem;
    m_word_t                    nfree;
};

/*
 * NOTES
 * -----
 * x86      -> cl = 32, blks = 256, page = 4096, slab = 8192
 * x86-64   -> cl = 64, blks = 512, page = 8192, slab = 32768
 * - ZEN_MEM_RUN_SLOTS must be <= 4/3 (32/64-bit) so the max size for
 * fastudiv16 is 65536
 */
#define ZEN_MEM_MIN_BLK         MACH_CL_SIZE
#define ZEN_MEM_MAX_BLK         (MACH_PAGE_SIZE / 2)
#define ZEN_MEM_MAX_SLAB_BLKS   (ZEN_MEM_BLK_SLAB_SIZE / MACH_CL_SIZE)
#define ZEN_MEM_BLK_SLAB_SIZE   (4 * MACH_PAGE_SIZE)
#define ZEN_MEM_MIN_RUN         MACH_PAGE_SIZE
#define ZEN_MEM_MAX_RUN         (MACH_PAGE_SIZE << (ZEN_MEM_RUN_SLOTS - 1))
#if (MACH_PAGE_SIZE == 4096)
#define ZEN_MEM_RUN_SLOTS       4
#elif (MACH_WORD_SIZE == 8)
#define ZEN_MEM_RUN_SLOTS       3
#define ZEN_MEM_RUN_SLAB_SIZE   (4 * ZEN_MEM_MAX_RUN)
#define zenmemblkpool(sz)       (fastudiv16(
struct zenmemslab {
    struct zenmemqueue         *queue;
    struct zenmemslab          *prev;
    struct zenmemslab          *next;
    volatile m_atomic_t         nfree;
    m_adr_t                     base;
    m_size_t                    size;
    m_word_t                    type;
    m_word_t                    flg;
    m_byte_t                    bmap[ZEN_MAX_SLAB_ITEMS / CHAR_BIT];
};

struct zenmembuf {
    volatile m_atomic_t         nref;
    m_adr_t                     ptr;
    m_size_t                    size;
    m_word_t                    flg;
};

#endif /* __SYS_ZEN_MEM_H__ */

