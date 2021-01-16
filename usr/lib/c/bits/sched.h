#ifndef __ZERO_BITS_SCHED_H__
#define __ZERO_BITS_SCHED_H__

#include <features.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#if defined(_POSIX_SPORADIC_SERVER) || defined(_POSIX_THREAD_SPORADIC_SERVER)
#include <time.h>
#endif
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>

/* __policy-field */
#define __SCHED_MIN_POLICY     SCHED_OTHER
#define SCHED_OTHER            0
#define SCHED_FIFO             1
#define SCHED_RR               2
#define SCHED_SPORADIC         3
#if defined(USEGNU) && (USEGNU)
#define SCHED_BATCH            4
#define SCHED_IDLE             5
#define __SCHED_NPOLICY        6
#define SCHED_RESET_ON_FORK    (1 << 31)
#else
#define __SCHED_NPOLICY        4
#endif
#define __SCHED_MAX_POLICY     (__SCHED_NPOLICY - 1)
/* __flg-field */
#define __SCHED_RT             (1 << 0) // realtime scheduling
#define __SCHED_FIXED_PRIORITY (1 << 1) // SYNC, INPUT, AUDIO, VIDEO
#define __SCHED_PROC_SCOPE     (1 << 2) // otherwise, system scope
struct sched_param {
    int             sched_priority;
    int             _pad;
#if defined(_POSIX_SPORADIC_SERVER) || defined(_POSIX_THREAD_SPORADIC_SERVER)
    int             sched_ss_low_priority;
    struct timespec sched_ss_repl_period;
    struct timespec sched_ss_init_budget;
    int             sched_ss_max_repl;
#endif /* SPORADIC */
    long            __policy;
    long            __nice;
    long            __flg;
    uint8_t         __res[2 * CLSIZE - 4 * sizeof(int) - 3 * sizeof(long)
                          - 2 * sizeof(struct timespec)];
};

#if defined(_GNU_SOURCE)

#define __CPU_SETSIZE  1024
#define __CPU_SETNCHAR (__CPU_SETSIZE / CHAR_BIT)
typedef struct {
    unsigned char __bitmap[__CPU_SETNCHAR];
} cpu_set_t;

typedef struct {
    unsigned char *__bitmap;
} __cpu_set_t;

#define CPU_ZERO(set)             CPU_ZERO_S(__CPU_SETNCHAR, set)
#define CPU_SET(set)              CPU_SET_S(__CPU_SETNCHAR, set)
#define CPU_CLR(set)              CPU_CLR_S(cpu, __CPU_SETNCHAR, set)
#define CPU_ISSET(set)            CPU_ISSET_S(cpu, __CPU_SETNCHAR, set)
#define CPU_COUNT(set)            CPU_COUNT_S(__CPU_SETNCHAR, set)

#define CPU_AND(dest, src1, src2) CPU_AND_S(__CPU_SETSIZE, dest, src1, src2)
#define CPU_OR(dest, src1, src2)  CPU_OR_S(__CPU_SETSIZE, dest, src1, src2)
#define CPU_XOR(dest, src1, src2) CPU_XOR_S(__CPU_SETSIZE, dest, src1, src2)
#define CPU_EQUAL(set1, set2)     CPU_EQUAL_S(__CPU_SETSIZE, set1, set2)

#define CPU_ALLOC(ncpu)       __sched_alloc_cpuset(ncpu)
#define CPU_ALLOC_SIZE(ncpu)  (ncpu * sizeof(char) / CHAR_BIT)
#define CPU_FREE(set)         free(set)

#define CPU_ZERO_S(sz, set)       memset(set, 0, sz)
#define CPU_SET_S(cpu, sz, set)   setbit(set->__bitmap, cpu)
#define CPU_CLR_S(cpu, sz, set)   clrbit(set->__bitmap, cpu)
#define CPU_ISSET_S(cpu, sz, set) bitset(set->__bitmap, cpu)
#define CPU_COUNT_S(sz, set)      __sched_count_cpus(set, __CPU_SETNCHAR)

#define CPU_AND_S(sz, dest, src1, src2)                                 \
    do {                                                                \
        unsigned long  ndx;                                             \
        unsigned long  lim = __CPU_SETSIZE / (sizeof(long) * CHAR_BIT); \
        unsigned long *destptr = (unsigned long *)(dest)->__bitmap;     \
        unsigned long *src1ptr = (unsigned long *)(src1)->__bitmap;     \
        unsigned long *src2ptr = (unsigned long *)(src2)->__bitmap;     \
                                                                        \
        for (ndx = 0 ; ndx < lim ; ndx++) {                             \
            destptr[ndx] = src1ptr[ndx] & src2ptr[ndx];                 \
        }                                                               \
    } while (0)
#define CPU_OR_S(sz, dest, src1, src2)                                  \
    do {                                                                \
        unsigned long  ndx;                                             \
        unsigned long  lim = __CPU_SETSIZE / (sizeof(long) * CHAR_BIT); \
        unsigned long *destptr = (unsigned long *)(dest)->__bitmap;     \
        unsigned long *src1ptr = (unsigned long *)(src1)->__bitmap;     \
        unsigned long *src2ptr = (unsigned long *)(src2)->__bitmap;     \
                                                                        \
        for (ndx = 0 ; ndx < lim ; ndx++) {                             \
            destptr[ndx] = src1ptr[ndx] | src2ptr[ndx];                 \
        }                                                               \
    } while (0)
#define CPU_XOR_S(sz, dest, src1, src2)                                 \
    do {                                                                \
        unsigned long  ndx;                                             \
        unsigned long  lim = __CPU_SETSIZE / (sizeof(long) * CHAR_BIT); \
        unsigned long *destptr = (unsigned long *)(dest)->__bitmap;     \
        unsigned long *src1ptr = (unsigned long *)(src1)->__bitmap;     \
        unsigned long *src2ptr = (unsigned long *)(src2)->__bitmap;     \
                                                                        \
        for (ndx = 0 ; ndx < lim ; ndx++) {                             \
            destptr[ndx] = src1ptr[ndx] ^ src2ptr[ndx];                 \
        }                                                               \
    } while (0)
#define CPU_EQUAL_S(sz, set1, set2) __sched_cpusets_equal(sz, set1, set2)

#endif /* defined(_GNU_SOURCE) */

#endif /* __ZERO_BITS_SCHED_H__ */

