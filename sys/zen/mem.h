#ifndef __SYS_ZEN_MEM_H__
#define __SYS_ZEN_MEM_H__

#include <mach/atomic.h>
#if defined(__v0__)
#include <zen/bsp/v0.h>
#endif

#define ZEN_ADR_LK_BIT_POS  0

struct zenmemconf {
    m_word_t    pagesize;
    m_word_t    hugesize;
    m_word_t    clsize;
    m_word_t    strsize;
};

struct zenmembuf {
    volatile m_atomic_t  nref;
    int8_t              *ptr;
    size_t               size;
    m_word_t             flg;
};

#endif /* __SYS_ZEN_MEM_H__ */

