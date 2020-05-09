#ifndef __TIME_H__
#define __TIME_H__

#if !defined(NULL)
#define NULL                    ((void *)0)
#endif
#if !defined(__time_t_defined)
typedef uint64_t                time_t;
#define __time_t_defined        1
#endif

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

struct timespec {
    time_t      tv_sec;
    long        tv_nsec;
};

struct itimerspec {
    struct timespec     it_interval;
    struct timespec     it_value;
};

#endif /* __TIME_H__ */

