#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#include <features.h>
#include <time.h>

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

#if !defined(__time_types_defined)
#include <bits/time.h>
#endif
#if !defined(__struct_timeval_defined)
#include <sys/bits/time.h>
#endif

#include <sys/types.h>

/* support for getitimer() and setitimer() */

#define ITIMER_REAL    0        // invoke SIGALRM on expiration
#define ITIMER_VIRTUAL 1        // invoke SIGVTALRM on expiration
#define ITIMER_PROF    2        // invoke SIGPROF on expiration

struct itimerval {
    struct timeval it_interval;
    struct timeval it_value;
};

#if defined(USEGNU)
#define TIMEVAL_TO_TIMESPEC(tv, ts)                                     \
    do {                                                                \
        (ts)->tv_sec = (tv)->tv_sec;                                    \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;                           \
    } while (0)
#define TIMESPEC_TO_TIMEVAL(tv, ts)                                     \
    do {                                                                \
        (tv)->tv_sec = (ts)->tv_sec;                                    \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;                           \
    } while (0)
#endif

#if defined(_FAVOR_BSD)
/* obsolete structure that should never be used */
struct timezone {
    int tz_minuteswest;         // minutes west of GMT
    int tz_dsttime;     // nonzero if DST is ever in effect
};
typedef struct timezone *__restrict timezone_ptr_t;
#else
typedef void            *__restrict timezone_ptr_t;
#endif

#if !defined(__zen__)

extern int gettimeofday(struct timeval *__restrict tv,
                        timezone_ptr_t tz);
#if defined(_BSD_SOURCE) && defined(_FAVOR_BSD)
extern int settimeofday(const struct timeval *tv, const struct timezone *tz);
extern int adjtime(const struct timeval *delta, struct timeval *olddelta);
#endif

typedef int itimer_which_t;

extern int getitimer(itimer_which_t, struct itimerval *val);
extern int setitimer(itimer_which_t, const struct itimerval *__restrict newval,
                     struct itimerval *__restrict oldval);
extern int utimes(const char *file, const struct timeval tv[2]);
extern int utimensat(int fd, const char *file, const struct timespec times[2],
                     int flg);
#if defined(_BSD_SOURCE)
extern int lutimes(const char *file, const struct timeval tv[2]);
extern int futimes(int fd, const struct timeval tv[2]);
#endif
#if defined(USEGNU)
extern int futimesat(int fd, const char *file, const struct timeval tv[2]);
#endif

#endif /* !defined(__KERNEL__) */

#if defined(_BSD_SOURCE)
#define    timerisset(tv) ((tv)->tv_sec || (tv)->tv_usec)
#define    timerclear(tv) (!(tv)->tv_sec && !(tv)->tv_usec);
#define    timercmp(tv1, tv2, _cmp)       \
    (((tv1)->tv_sec == (tv2)->tv_sec)          \
     ? ((tv1)->tv_usec CMP (tv2)->tv_usec)     \
     : ((tv1)->tv_sec CMP (tv2)->tv_sec))
# define   timeradd(tv1, tv2, res)                                      \
    do {                                                                \
        (res)->tv_sec = (tv1)->tv_sec + (tv2)->tv_sec;                  \
        (res)->tv_usec = (tv1)->tv_usec + (tv2)->tv_usec;               \
        if ((res)->tv_usec >= 1000000) {                                \
            ++(result)->tv_sec;                                         \
            (result)->tv_usec -= 1000000;                               \
        }                                                               \
    } while (0)
# define   timersub(tv1, b, result)                                     \
    do {                                                                \
        (res)->tv_sec = (tv1)->tv_sec - (tv2)->tv_sec;                  \
        (res)->tv_usec = (tv1)->tv_usec - (tv2)->tv_usec;               \
        if ((result)->tv_usec < 0) {                                    \
            --(res)->tv_sec;                                            \
            (res)->tv_usec += 1000000;                                  \
        }                                                               \
    } while (0)
#endif /* USEBSD */

//#include <time.h>
//#include <sys/select.h>

#endif /* __SYS_TIME_H__ */

