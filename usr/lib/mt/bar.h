#ifndef MT_BAR_H
#define MT_BAR_H

#include <mt/conf.h>

#if defined(MT_BARRIER)

#include <mt/mtx.h>
#include <mt/cond.h>

/* velho thread barriers */

#if (MACH_WORD_SIZE == 8)
#define ZEN_BARFLAGBIT              (UINT64_C(1) << 63)
#elif (MACH_WORD_SIZE == 4)
#define ZEN_BARFLAGBIT              (UINT32_C(1) << 31)
#endif
#define ZEN_BARSERIALTHR            (-1L)

typedef struct __zenbar {
    zenfmtx                     lk;
    unsigned long               num;
    unsigned long               cnt;
    zencond                     cond;
} zenbar;

typedef struct __zenbarpool {
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
} zenbarpool;

#endif /* defined(MT_BARRIER) */

#endif /* MT_BAR_H */

