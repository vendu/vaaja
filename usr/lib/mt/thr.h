#ifndef MT_THR_H
#define MT_THR_H

/* zen thread abstraction */

#if defined(POSIX_THREAD) && !defined(MT_ZEN_THREAD)
#define thrself()                   ((uintptr_t)pthread_self())
#endif

#if defined(__zen__)
#define zenyieldthr()               schedyield()
#elif defined(POSIX_THREAD)
#define zenyieldthr()               pthread_yield()
#elif defined(_WIN64) || defined(_WIN32)
#define zenyieldthr()               kYieldProcessor()
#elif defined(__linux__) && !defined(__KERNEL__)
#define zenyieldthr()               sched_yield()
#endif

#if defined(ZEN_THREAD)

typedef uintptr_t zenthr;

#define ZEN_THRATR_INIT             (1 << 0)    // attributes initialised
#define ZEN_THRATR_DETACHED         (1 << 1)    // detach thread
#define ZEN_THRATR_INHERITSCHED     (1 << 2)    // inherit scheduler parameters
#define ZEN_THRATR_EXPLICITSCHED    (1 << 3)    // specify scheduler parameters
#define ZEN_THRATR_SCHED_PARAM      (1 << 4)    // scheduler parameters
#define ZEN_THRATR_SCHEDPOLICY      (1 << 5)    // scheduler policy
#define ZEN_THRATR_SCOPE            (1 << 6)    // scheduling scope
#define ZEN_THRATR_STKATR           (1 << 7)    // stack address and size
#define ZEN_THRATR_GUARDSIZE        (1 << 8)    // stack guard size
#define ZEN_THRATR_AFFINITY         (1 << 9)    // affinity configuration
typedef struct __zenthratr {
    long                            flg;
    void                           *stkadr;
    size_t                          stksize;
    size_t                          guardsize;
#if defined(_GNU_SOURCE)
    size_t                          ncpu;
    void                           *cpuset;
#endif
#if defined(ZEN_SCHED_PARAM)
    struct sched_param              schedparm;
#endif
} zenthratr;

#define ZEN_THR_NOID                (~(zenthr)0)
#define ZEN_THR_ASLEEP              1
#define ZEN_THR_AWAKE               0
typedef struct __zenthr             {
    zenthr                          id;
    long                            sleep;
    zenthratr                      *atr;
    struct __zenthr                *prev;
    struct __zenthr                *next;
} zenthr;

#define ZEN_THRQUEUE_INITIALIZER    { MTXINITVAL, NULL, NULL }
typedef struct __zenthrqueue {
    mtfmtx                          mtx;
    zenthr                         *head;
    zenthr                         *tail;
    uint8_t                         _pad[MACH_CL_SIZE
                                         - MACH_WORD_SIZE
                                         - 2 * MACH_PTR_SIZE];
} zenthrqueue;
#define ZEN_THRQUEUE_SIZE           (MACH_CL_SIZE                      \
                                     + MACH_WORD_SIZE                  \
                                     + 2 * MACH_PTR_SIZE               \
                                     + MACH_CL_SIZE                    \
                                     + MACH_CL_SIZE                    \
                                     - MACH_WORD_SIZE                  \
                                     - 2 * MACH_PTR_SIZE)

extern void                         zenwaitthr(zenthrqueue *queue);
extern long                         zensleepthr(zenthrqueue *queue,
                                                const struct timespec *absts);
extern zenthr                       *zenwakethr(zenthrqueue *queue);
extern void                         zenwakethrall(zenthrqueue *queue);

#define zenwaitthr()                zenthrwait1(NULL)

#endif /* defined(ZEN_THREAD) */

#endif /* MT_THR_H */

