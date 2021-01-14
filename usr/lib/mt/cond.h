#ifndef MT_COND_H
#define MT_COND_H

/* velho conditional variables */

#if defined(MT_CONDITION_VAR)

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

