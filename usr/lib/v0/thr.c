#include <v0/v0.h>
#include <zero/cdefs.h>
#if defined(V0_PTHREAD)
#include <pthread.h>
#endif

THREADLOCAL struct v0thr          t_ctx;
#if defined(V0_PTHREAD)
THREADLOCAL pthread_t             t_thrid;
THREADLOCAL pthread_mutex_t       t_waitmtx;
THREADLOCAL void                * t_exitval;

static __inline__ void
v0initthr(struct v0thr *thr)
{
    v0bzerow(thr, sizeof(struct v0thr));
    thr->sysregs[V0_MSW_REG] = V0_MSW_DEF_BITS;

    return;
}

