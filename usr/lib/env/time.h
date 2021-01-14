#ifndef __ZERO_TIME_H__
#define __ZERO_TIME_H__

#include <features.h>
#include <time.h>

#define __timespectotm(tp) ((tp)->tv_sec * CLOCKS_PER_SEC + (tp)->tv_nsec)

#if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199309L)
static __inline__ time_t
timegetstamp(void)
{
    struct timespec ts;
    time_t          tm;

#if defined(CLOCK_MONOTONIC)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    tm = __timespectotm(&ts);

    return tm;
}
#endif /* _POSIX_C_SOURCE */

#if defined(_ZERO_SOURCE)

#define timevalcmp(tv1, tv2)                                            \
    (((tv2)->tv_sec - (tv1)->tv_sec) * 1000000                          \
     + ((tv2)->tv_usec - (tv1)->tv_usec))
#define timevalgt(tv1, tv2)                                             \
    (((tv1)->tv_sec > (tv2)->tv_sec)                                    \
     || ((tv1)->tv_sec == (tv2)->tv_sec && (tv1)->tv_usec > (tv2)->tv_usec))

#define timespeccmp(ts1, ts2)                                           \
    (((ts2)->tv_sec - (ts1)->tv_sec) * CLOCKS_PER_SEC                   \
     + (time_t)(ts2)->tv_nsec - (time_t)(ts1)->tv_nsec)
#define timespecgt(ts1, ts2)                                             \
    (((ts1)->tv_sec > (ts2)->tv_sec)                                    \
     || ((ts1)->tv_sec == (ts2)->tv_sec && (ts1)->tv_nsec > (ts2)->tv_nsec))

#endif /* _ZERO_SOURCE */

#endif /* __ZERO_TIME_H__ */

