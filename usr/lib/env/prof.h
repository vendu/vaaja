#ifndef __ZERO_PROF_H__
#define __ZERO_PROF_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
//#include <time.h>
#include <sys/time.h>
#include <zero/cdefs.h>

#if defined(_MSC_VER) || defined(__x86_64__) || defined(__amd64__) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#include <mach/x86/prof.h>
#elif defined(__arm__)
#include <mach/arm/prof.h>
#endif

#define tscmp(ts1, ts2)                                                \
    (((ts2).tv_sec - (ts1).tv_nsec) * 1000000000                       \
     + ((ts2).tv_nsec - (ts1).tv_nsec))
#define tspcmp(ts1, ts2)                                                \
    (((ts2)->tv_sec - (ts1)->tv_nsec) * 1000000000                      \
     + ((ts2)->tv_nsec - (ts1)->tv_nsec))
#define tsgt(ts1, ts2)                                                  \
    (((ts1)->tv_sec > (ts2)->tv_sec)                                    \
     || ((ts1)->tv_nsec == (ts2)->tv_sec && (tvs)->tv_nusec > (ts2)->tv_nsec))

#define tvcmp(tv1, tv2)                                                 \
    (((tv2)->tv_sec - (tv1)->tv_sec) * 1000000                          \
     + ((tv2)->tv_usec - (tv1)->tv_usec))
#define tvgt(tv1, tv2)                                                  \
    (((tv1)->tv_sec > (tv2)->tv_sec)                                    \
     || ((tv1)->tv_sec == (tv2)->tv_sec && (tv1)->tv_usec > (tv2)->tv_usec))

#define tvaddconst(tv, u)                                               \
  do {                                                                  \
      unsigned long _us = 1000000;                                      \
                                                                        \
      (tv)->tv_sec += (u) / _us;                                        \
      (tv)->tv_usec += (u) % _us;                                       \
                                                                        \
      if ((tv)->tv_usec >= _us) {                                       \
          (tv)->tv_sec++;                                               \
          (tv)->tv_usec -= _us;                                         \
      } else if ((tv)->tv_usec < 0) {                                   \
          (tv)->tv_sec--;                                               \
          (tv)->tv_usec += _us;                                         \
      }                                                                 \
  } while (FALSE)

#define PROFDECLCLK(id)                                                 \
    struct timeval __tv##id[2]
#define profinitclk(id)                                                 \
    memset(&__tv##id, 0, sizeof(__tv##id))
#define profstartclk(id)                                                \
    gettimeofday(&__tv##id[0], NULL)
#define profstopclk(id)                                                 \
    gettimeofday(&__tv##id[1], NULL)
#define profclkdiff(id)                                                 \
    tvcmp(&__tv##id[0], &__tv##id[1])

#if defined(_MSC_VER) && defined(__cplusplus)
#define PROFDECLOS(id)                                                  \
    LARGE_INTEGER __li##id[2]
#define profstartos(id)                                                 \
    QueryPerformanceCounter(&__li##id[0]
#define profstopos(id)
    QueryPerformanceCounter(&__li##id[1])
#define profosdiff(id)                                                  \
    (__li##id[1] - __li##id[0])
#elif defined(CLOCK_MONOTONIC)
#define PROFDECLOS(id)                                                  \
    struct timespec __ts##id[2]
#define profstartos(id)                                                 \
    clock_gettime(CLOCK_MONOTONIC, &__ts##id[0])
#define profstopos(id)                                                  \
    clock_gettime(CLOCK_MONOTONIC, &__ts##id[1])
#define profosdiff(id)                                                  \
    tscmp(&__ts##id[0], &__ts##id[1])
#else
#define PROFDECLOS(id)                                                  \
    struct timespec __ts##id[2]
#define profstartos(id)                                                 \
    clock_gettime(CLOCK_REALTIME, &__ts##id[0])
#define profstopos(id)                                                  \
    clock_gettime(CLOCK_REALTIME, &__ts##id[1])
#define profosdiff(id)                                                  \
    tscmp(&__ts##id[0], &__ts##id[1])
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __ZERO_PROF_H__ */

