#ifndef __TIME_H__
#define __TIME_H__

#if !defined(NULL)
#define NULL                    ((void *)0)
#endif
#include <api/zen/types.h>
#include <sys/types.h>
#include <share/time.h>

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

#define TIMESPEC_SIZE           (M_TIME_T_SIZE + 2 * MACH_INT_sIZE)
struct timespec {
    time_t      tv_sec;
    int         tv_nsec;
};

struct itimerspec {
    struct timespec     it_interval;
    struct timespec     it_value;
};

#endif /* __TIME_H__ */

