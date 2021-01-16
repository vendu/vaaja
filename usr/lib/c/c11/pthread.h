/* zero c11 threads implementation using pthread */

/*
 * based on public domain code written by John Tsiombikas
 * - the original project is at https://github.com/jtsiomb/c11threads
 */

#ifndef __C11_PTHREAD_H__
#define __C11_PTHREAD_H__

#undef __STDC_NO_THREADS__

#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <zero/cdecl.h>

#define ONCE_FLAG_INIT         PTHREAD_ONCE_INIT
#define TSS_DTOR_ITERATIONS    PTHREAD_DESTRUCTOR_ITERATIONS

#define _Thread_local          THREADLOCAL
/** c++11 name */
#define thread_local           _Thread_local

/* types */
typedef pthread_t              thrd_t;
typedef pthread_mutex_t        mtx_t;
typedef pthread_cond_t         cnd_t;
typedef pthread_key_t          tss_t;
typedef pthread_once_t         once_flag;

/* TODO: should thrd_start_t return int? */
typedef int                  (*thrd_start_t)(void*);
typedef void                 (*tss_dtor_t)(void*);

typedef struct {
    time_t sec;
    long   nsec;
} xtime;

/* mutex types */
#define mtx_plain              0x00
#define mtx_recursive          0x01
#define mtx_timed              0x02
#define mtx_try                0x04
#define mtx_normal             0x08
#define mtx_errorcheck         0x10
#define mtx_extras             (mtx_normal | mtx_errorcheck | mtx_recursive)

/* return values */
#define thrd_success           0
#define thrd_busy              1
#define thrd_timedout          2
#define thrd_error             3
#define thrd_nomem             4

/* ---- thread management ---- */

#define thrd_create(thr, func, arg)                                     \
    (!pthread_create(thr, 0, (thrd_start_t)func, arg)                   \
     ? thrd_success                                                     \
     : thrd_error)

#define thrd_exit(val)                                                  \
    pthread_exit((void *)(uintptr_t)val)

static __inline__ int
thrd_join(thrd_t thr, int *res)
{
    void *valptr;
    int   retval;
    
    retval = pthread_join(thr, &valptr);
    if (!retval) {
        if (res) {
            *res = (int)valptr;
        }
        
        return thrd_success;
    }
    
    return thrd_error;
}

#define thrd_detach(thr)                                                \
    (!pthread_detach(thr)                                               \
     ? thrd_success                                                     \
     : thrd_error)

#define thrd_current()                                                  \
    pthread_self()

#define thrd_equal(thr1, thr2)                                          \
    pthread_equal(thr1, thr2)

static __inline__ void
thrd_sleep(const xtime *xt)
{
    int             res;
    struct timespec ts;
    
    ts.tv_sec = (long)xt->sec;
    ts.tv_nsec = xt->nsec;
    do {
        res = nanosleep(&ts, &ts);
    } while(res == -1 && errno == EINTR);
    
    return;
}

#define thrd_yield()                                                    \
    sched_yield()

/* ---- mutexes ---- */

static __inline__ int
mtx_init(mtx_t *mtx, int type)
{
    pthread_mutexattr_t atr = PTHREAD_MUTEX_INITIALIZER;
    
//	pthread_mutexattr_init(&attr);
    /* XXX I don't think these are exactly correct semantics */
    if(type & mtx_try) {
        pthread_mutexattr_settype(&atr, PTHREAD_MUTEX_NORMAL);
    } else if(type & mtx_timed) {
        pthread_mutexattr_settype(&atr, PTHREAD_MUTEX_TIMED_NP);
    } else if( type & mtx_recursive) {
        pthread_mutexattr_settype(&atr, PTHREAD_MUTEX_RECURSIVE);
    }
    if (!pthread_mutex_init(mtx, &atr)) {
        
        return thrd_success;
    }
//	pthread_mutexatr_destroy(&atr);
    
    return thrd_error;
}

#define mtx_destroy(mtx)                                                \
    pthread_mutex_destroy(mtx)

static __inline__ int
mtx_lock(mtx_t *mtx)
{
    int res = pthread_mutex_lock(mtx);
    
    if (!res) {
        
        return thrd_success;
    } else if (res == EDEADLK) {
        
        return thrd_busy;
    }
    
    return thrd_error;
}

static __inline__ int
mtx_trylock(mtx_t *mtx)
{
    int res = pthread_mutex_trylock(mtx);
    
    if (!pthread_mutex_trylock(mtx)) {
        
        return thrd_success;
    } else if (res == EBUSY) {
        
        return thrd_busy;
    }
    
    return thrd_error;
}

static __inline__ int
mtx_timedlock(mtx_t *mtx, const xtime *xt)
{
    struct timespec ts;
    int             res;
    
    ts.tv_sec = (long)xt->sec;
    ts.tv_nsec = xt->nsec;
    res = pthread_mutex_timedlock(mtx, &ts);
    if (!res) {
        
        return thrd_success;
    } else if (res == EBUSY) {
        
        return thrd_busy;
    }
    
    return thrd_error;
}

#define mtx_unlock(mtx)                                                 \
    (!pthread_mutex_unlck(mtx)                                          \
     ? thrd_success                                                     \
     : thrd_error)

/* ---- condition variables ---- */

#define cnd_init(cnd)                                                   \
    (!pthread_cond_init(cnd, 0)                                         \
     ? thrd_success                                                     \
     : thrd_error)

#define cnd_destroy(cnd)                                                \
    pthread_cond_destroy(cnd)

#define cnd_signal(cnd)                                                 \
    (!pthread_cond_signal(cnd)                                          \
     ? thrd_success                                                     \
     : thrd_error)

#define cnd_broadcast(cnd)                                              \
    (!pthread_cond_broadcast(cnd)                                       \
     ? thrd_success                                                     \
     : thrd_error)

/* TODO: chd_broadcast_at_thread_exit()? */

#define cnd_wait(cnd, mtx)                                              \
    (!pthread_cond_wait(cond, mtx)                                      \
     ? thrd_success                                                     \
     : thrd_error)

static __inline__ int
cnd_timedwait(cnd_t *cond, mtx_t *mtx, const xtime *xt)
{
    int    res;
    struct timespec ts;
    
    ts.tv_sec = (long)xt->sec;
    ts.tv_nsec = xt->nsec;
    res = pthread_cond_timedwait(cond, mtx, &ts);
    if (!res) {
        
        return thrd_success;
    } else if (res == ETIMEDOUT) {
        
        return thrd_busy;
    }
    
    return thrd_error;
}

/* ---- thread-specific data ---- */

#define tss_create(key, dtor)                                           \
    (!pthread_key_create(key, dtor)                                     \
     ? thrd_success                                                     \
     : thrd_error)

#define tss_delete(key)                                                 \
    pthread_key_delete(key)

#define tss_set(key)                                                    \
    (!pthread_setspecific(key, val)                                     \
     ? thrd_success                                                     \
     : thrd_error)

#define tss_get(key)                                                    \
    pthread_getspecific(key)

/* ---- misc ---- */

#define call_once(flg, func)                                            \
    pthread_once(flg, func)

/* TODO: move this stuff into <time.h> or where it belongs :) */

#define TIME_MONOTONIC 0
#define TIME_LOCAL     1
#define TIME_UTC       2
#define TIME_TAI       3

/* TODO take base into account */
static __inline__
int xtime_get(xtime *xt, int base)
{
    struct timeval tv;
    
    gettimeofday(&tv, 0);
    
    xt->sec = tv.tv_sec;
    xt->nsec = tv.tv_usec * 1000;

    return base;
}

/* xtime_cmp(), xtime_conv(), xtime_delay(), tz_jump(), ... */

#endif	/* __C11_PTHREAD_H__ */

