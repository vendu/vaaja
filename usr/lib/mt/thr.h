#ifndef __MT_THR_H__
#define __MT_THR_H__

/* velho thread abstraction */

#include <mt/conf.h>
#include <stddef.h>
#include <stdint.h>
#include <mt/mtx.h>

#if defined(PTHREAD) && !defined(__KERNEL__)
#define mtthrself()         ((uintptr_t)pthread_self())
#endif

#if defined(VELHO)
#include <sched.h>
#define mtyieldthr()    sched_yield()
#elif defined(MTTHREAD) || (POSIX_THREAD)
#define mtyieldthr()    pthread_yield()
#elif defined(_WIN64) || defined(_WIN32)
#define mtyieldthr()    kYieldProcessor()
#elif defined(__linux__) && !defined(__KERNEL__)
#define mtyieldthr()    sched_yield()
#elif defined(__KERNEL__)
#define mtyieldthr()    schedyield()
#endif

#if defined(MTTHREAD)

#if defined(MTSCHED)
#include <sched.h>
#endif

typedef uintptr_t mtthrid;

#define MT_THRATR_INIT          (1 << 0)       // attributes initialised
#define MT_THRATR_DETACHED      (1 << 1)       // detach thread
#define MT_THRATR_INHERITSCHED  (1 << 2)       // inherit scheduler parameters
#define MT_THRATR_EXPLICITSCHED (1 << 3)       // specify scheduler parameters
#define MT_THRATR_SCHED_PARAM   (1 << 4)       // scheduler parameters
#define MT_THRATR_SCHEDPOLICY   (1 << 5)       // scheduler policy
#define MT_THRATR_SCOPE         (1 << 6)       // scheduling scope
#define MT_THRATR_STKATR        (1 << 7)       // stack address and size
#define MT_THRATR_GUARDSIZE     (1 << 8)       // stack guard size
#define MT_THRATR_AFFINITY      (1 << 9)       // affinity configuration
typedef struct __mtthratr {
    long                  flg;
    void                 *stkadr;
    size_t                stksize;
    size_t                guardsize;
#if defined(_GNU_SOURCE)
    size_t                ncpu;
    void                *cpuset;
#endif
#if defined(MTSCHED)
    struct sched_param   schedparm;
#endif
} mtthratr;

#define MT_THR_NOID     (~(mtthrid)0)
#define MT_THR_ASLEEP   1
#define MT_THR_AWAKE    0
typedef struct __mtthr {
    mtthrid             id;
    long                sleep;
    mtthratr           *atr;
    struct __mtthr     *prev;
    struct __mtthr     *next;
} mtthr;

#define MT_THRQUEUE_INITIALIZER { MTXINITVAL, NULL, NULL }
typedef struct __mtthrqueue {
    mtfmtx      mtx;
    mtthr      *head;
    mtthr      *tail;
} mtthrqueue;

extern void     mtwaitthr1(mtthrqueue *queue);
extern long     mtsleepthr2(mtthrqueue *queue, const struct timespec *absts);
extern mtthr  * mtwakethr1(mtthrqueue *queue);
extern void     mtwakethrall(mtthrqueue *queue);

#define         mtwaitthr()     thrwait1(NULL)

#endif /* defined(MTTHREAD) */

#endif /* __MT_THR_H__ */

