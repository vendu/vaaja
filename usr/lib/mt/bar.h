#ifndef MT_BAR_H
#define MT_BAR_H

#include <mt/conf.h>

#if defined(MT_BARRIER)

#include <mt/mtx.h>
#include <mt/cond.h>

/* velho thread barriers */

#if (MACH_WORD_SIZE == 8)
#define BARFLAGBIT              (UINT64_C(1) << 63)
#elif (MACH_WORD_SIZE == 4)
#define BARFLAGBIT              (UINT32_C(1) << 31)
#endif
#define BARSERIALTHR (-1L)

typedef struct __mtbar {
    zenfmtx                     lk;
    unsigned long               num;
    unsigned long               cnt;
    zencond                     cond;
} mtbar;

typedef struct __mtbarpool {
    zenfmtx                     lk;
    m_atomic_t                  nref;
    long                        num;
    union {
        struct {
            volatile uint32_t   seq;
            volatile uint32_t   cnt;
        } vals;
        volatile uint64_t       rst;
    } cnt;
} mtbarpool;

#endif /* defined(MT_BARRIER) */

#endif /* MT_BAR_H */

