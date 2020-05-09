#ifndef __MT_BAR_H__
#define __MT_BAR_H__

/* velho thread barriers */

#define MTFMTX 1

#include <stdint.h>
#include <mach/param.h>
#include <mt/mtx.h>
#include <mt/cond.h>

#if (LONGSIZE == 8)
#define BARFLAGBIT   (UINT64_C(1) << 63)
#elif (LONGSIZE == 4)
#define BARFLAGBIT   (UINT32_C(1) << 31)
#endif
#define BARSERIALTHR (-1L)

#define BAR_LK_T mtfmtx

typedef struct __mtbar {
    BAR_LK_T      lk;
    unsigned long num;
    unsigned long cnt;
    mtcond      cond;
} mtbar;

typedef struct __mtbarpool {
    BAR_LK_T                  lk;
    m_atomic_t                nref;
    long                      num;
    union {
        struct {
            volatile uint32_t seq;
            volatile uint32_t cnt;
        } vals;
        volatile uint64_t     rst;
    } cnt;
} mtbarpool;

#endif /* __MT_BAR_H__ */

