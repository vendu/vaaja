#ifndef __MT_COND_H__
#define __MT_COND_H__

/* velho conditional variables */

#if defined(PTHREAD)
#include <stddef.h>
#include <time.h>
#include <pthread.h>
#endif
#include <mt/conf.h>
#include <mt/mtx.h>
#include <mt/thr.h>

#if 0 && defined(PTHREAD) && !defined(MTCOND)

typedef pthread_cond_t mtcond;

#define mtinitcond(cp)                  pthread_cond_init(cp, NULL)
#define mtwaitcond(cp, mp)              pthread_cond_wait(cp, mp)
#define mtwaitcondtimed(cp, mp, tsp)    pthread_cond_timedwait(cp, mp, tsp)
#define mtsigcondone(cp)                pthread_cond_signal(cp)
#define mtsigcondmany(cp, n)            pthread_cond_broadcast(cp)
#define mtsigcondall(cp)                pthread_cond_broadcast(cp)
#define mtfreecond(cp)                  pthread_cond_destroy(cp)

#elif defined(MTCOND)

#define MTCOND_INITIALIZER { MTTHRQUEUE_INITIALIZER }
typedef struct __mtcond {
    mtthrqueue queue;
} mtcond;

#endif

#define mtsigcond(cp)   mtsigcondmany(cp, 8)

void    mtinitcond(mtcond *cond);
long    mtsigcondone(mtcond *cond);
long    mtsigcondmany(mtcond *cond, long nthr);
long    mtsigcondall(mtcond *cond);
#if (MTFMTX)
long    mtwaitcond(mtcond *cond, mtfmtx *mtx);
long    mtwaitcondabstime(mtcond *cond, mtfmtx *mtx,
                          const struct timespec *absts);
#elif (MTMTX)
long    mtwaitcond(mtcond *cond, mtmtx *mtx);
long    mtwaitcondabstime(mtcond *cond, mtmtx *mtx,
                          const struct timespec *absts);
#endif

#endif /* __MT_COND_H__ */

