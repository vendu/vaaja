#ifndef MT_BAR_H
#define MT_BAR_H

/* velho thread barriers */

#if (MACH_WORD_SIZE == 8)
#define BARFLAGBIT              (UINT64_C(1) << 63)
#elif (MACH_WORD_SIZE == 4)
#define BARFLAGBIT              (UINT32_C(1) << 31)
#endif
#define BARSERIALTHR (-1L)

typedef mtfmtx                  zenbarlk;

typedef struct __mtbar {
    zenbarlk                    lk;
    unsigned long               num;
    unsigned long               cnt;
    zencond                     cond;
} mtbar;

typedef struct __mtbarpool {
    zenbarlk                    lk;
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

#endif /* MT_BAR_H */

