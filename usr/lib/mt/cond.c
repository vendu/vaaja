#include <stddef.h>
#include <time.h>
#include <errno.h>
#if defined(PTHREAD)
#include <pthread.h>
#endif
#include <mt/mtx.h>
#include <mt/cond.h>
#include <mt/thr.h>

/* initialise condition variable */
void
mtinitcond(mtcond *cond)
{
#if (MTFMTX)
    mtinitfmtx(&cond->queue.mtx);
#endif
    cond->queue.head = NULL;
    cond->queue.tail = NULL;

    return;
}

/* wake one thread up */
long
mtsigcondone(mtcond *cond)
{
    if (!cond) {

        return -EINVAL;
    }
    mtwakethr1(&cond->queue);

    return 0;
}

/* wake several threads up */
long
mtsigcondmany(mtcond *cond, long nthr)
{
    if (!cond || !nthr) {

        return -EINVAL;
    }
    while (nthr--) {
        if (!mtwakethr1(&cond->queue)) {

            return 0;
        }
    }

    return 0;
}

long
mtsigcondall(mtcond *cond)
{
    if (!cond) {

        return EINVAL;
    }
    mtwakethrall(&cond->queue);

    return 0;
}

long
mtwaitcond(mtcond *cond, mtfmtx *fmtx)
{
    if (!cond || !fmtx) {

        return -EINVAL;
    } else if (!mttryfmtx(fmtx)) {

        return -EPERM;
    } else {
        mtwaitthr1(&cond->queue);
    }
    mtlkfmtx(fmtx);

    return 0;
}

long
mtwaitcondabstime(mtcond *cond, mtfmtx *fmtx, const struct timespec *absts)
{
    if (!cond || !fmtx || !absts || absts->tv_sec < 0
        || absts->tv_nsec < 0 || absts->tv_nsec >= 1000000000) {

        return -EINVAL;
    } else if (!mttryfmtx(fmtx)) {

        return -EPERM;
    } else if (mtsleepthr2(&cond->queue, absts) < 0) {

        return -EINVAL;
    }
    mtlkfmtx(fmtx);

    return 0;
}

