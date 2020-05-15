#ifndef __SYS_ZEN_MEM_H__
#define __SYS_ZEN_MEM_H__

#if defined(__v0__)
#include <zen/sys/v0.h>
#endif

struct zenmemconf {
    zenlong     pagesize;
    zenlong     hugesize;
    zenlong     clsize;
    zenlong     strsize;
};

struct zenmembuf {
    volatile m_atomic_t  nref;
    int8_t              *ptr;
    size_t               size;
    m_word_t             flg;
};

#endif /* __SYS_ZEN_MEM_H__ */

