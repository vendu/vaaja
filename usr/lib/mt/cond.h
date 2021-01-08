#ifndef MT_COND_H
#define MT_COND_H

/* velho conditional variables */

#if defined(POSIX_THREAD)
#include <stddef.h>
#include <time.h>
#include <pthread.h>
#endif
#include <thr/conf.h>
#include <thr/mtx.h>
#include <thr/thr.h>

#if defined(POSIX_THREAD) && !defined(MTCOND)

typedef pthread_cond_t mtcond;

#define mtinitcond(cp)                 pthread_cond_init(cp, NULL)
#define mtwaitcond(cp, mp)             pthread_cond_wait(cp, mp)
#define mtwaitcondtimed(cp, mp, tsp)   pthread_cond_timedwait(cp, mp, tsp)
#define mtsigcondone(cp)               pthread_cond_signal(cp)
#define mtsigcondmany(cp, n)           pthread_cond_broadcast(cp)
#define mtsigcondall(cp)               pthread_cond_broadcast(cp)
#define mtfreecond(cp)                 pthread_cond_destroy(cp)

#elif defined(MTCOND)

#define MTCOND_INITIALIZER { MTTHRQUEUE_INITIALIZER }
typedef struct __mtcond {
    thrqueue                            queue;
} mtcond;

#endif

#define mtsigcond(cp)                   mtsigcondmany(cp, 8)

void                                    mtinitcond(mtcond *cond);
long                                    mtsigcondone(mtcond *cond);
long                                    mtsigcondmany(mtcond *cond, long nmt);
long                                    mtsigcondall(mtcond *cond);
long                                    mtwaitcond(mtcond *cond, mtmtx *thrx);
long                                    mtwaitcondabstime(thrcond *cond,
                                                          thrthrx *thrx,
                                                          const struct timespec *absts);

#endif /* MT_COND_H */

