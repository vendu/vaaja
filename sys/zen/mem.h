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
    m_word_t    pagesize;
    m_word_t    hugesize;
    m_word_t    clsize;
    m_word_t    strsize;
};

struct zenmemqueue {
    void                  (*free)(void *);
    void                   *head;
    void                   *tail;
    volatile m_atomic_t     nref;
    m_word_t                type;
    m_word_t                slot;
    m_word_t                nitem;
};

/*
 * x86      -> cl = 32, blks = 256, page = 4096, slab = 8192
 * x86-64   -> cl = 64, blks = 512, page = 8192, slab = 32768
 */
#define ZEN_MEM_MIN_BLK     MACH_CL_SIZE
#define ZEN_MEM_MAX_BLK     (MACH_PAGE_SIZE / 2)
#define ZEN_MAX_SLAB_BLKS   (MACH_CL_SIZE * CHAR_BIT)
#define ZEN_MEM_SLAB_SIZE   (ZEN_MIM_MEM_BLK * ZEN_MAX_SLAB_BLKS)
struct zenmemslab {
    struct zenmemqueue     *queue;
    volatile m_atomic_t     mtx;
    volatile m_atomic_t     nref;
    struct zenmemslab      *prev;
    struct zenmemslab      *next;
    m_adr_t                 base;
    m_size_t                size;
    m_word_t                type;
    m_byte_t                bmap[C_VLA];
};

struct zenmembuf {
    volatile m_atomic_t  nref;
    m_adr_t              ptr;
    m_size_t             size;
    m_word_t             flg;
};

#endif /* __SYS_ZEN_MEM_H__ */

