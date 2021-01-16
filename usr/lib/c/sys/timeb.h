#ifndef __SYS_TIMEB_H__
#define __SYS_TIMEB_H__

#include <time.h>
#include <sys/types.h>
#include <zero/trix.h>

struct timeb {
    time_t         time;
    unsigned short millitm;
    short          timezone;
    short          dstflag;
};

int ftime(struct timeb *tb);

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199309L)
static __inline__ int
ftime(struct timeb *tb)
{
    time_t          sec;
    time_t          nsec;
    unsigned short  msec;
    struct timespec ts;

#if defined(CLOCK_MONOTONIC)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    nsec = ts->tv_nsec;
    sec = ts->tv_sec;
    msec = divu1000000(nsec);
    tb->sec = sec;
    tb->millitm = msec;

    return 0;
}
#endif /* POSIX */

#endif /* __SYS_TIMEB_H__ */

