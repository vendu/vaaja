#ifndef MT_COND_H
#define MT_COND_H

/* velho conditional variables */

#if defined(POSIX_THREAD) && !defined(MTCOND)

typedef pthread_cond_t                  zencond;

#define mtinitcond(cp)                  pthread_cond_init(cp, NULL)
#define mtwaitcond(cp, mp)              pthread_cond_wait(cp, mp)
#define mtwaitcondtimed(cp, mp, tsp)    pthread_cond_timedwait(cp, mp, tsp)
#define mtsigcondone(cp)                pthread_cond_signal(cp)
#define mtsigcondmany(cp, n)            pthread_cond_broadcast(cp)
#define mtsigcondall(cp)                pthread_cond_broadcast(cp)
#define mtfreecond(cp)                  pthread_cond_destroy(cp)

#elif defined(MTCOND)

#define MTCOND_INITIALIZER { MTTHRQUEUE_INITIALIZER }
typedef struct __zencond {
    zenthrqueue                         queue;
} zencond;

#endif

#define mtsigcond(cp)                   mtsigcondmany(cp, 8)

void                                    mtinitcond(zencond *cond);
long                                    mtsigcondone(zencond *cond);
long                                    mtsigcondmany(zencond *cond, long nmt);
long                                    mtsigcondall(zencond *cond);
long                                    mtwaitcond(zencond *cond, mtmtx *thrx);
long                                    mtwaitcondabstime(zencond *cond,
                                                          mtfmtx *fmtx,
                                                          const struct timespec *absts);

#endif /* MT_COND_H */

