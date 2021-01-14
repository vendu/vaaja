#ifndef __SYS_BITS_TIME_H__
#define __SYS_BITS_TIME_H__

#if !defined(__struct_timeval_defined)
struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
};
#define __struct_timeval_defined 1
#endif

#endif /* __SYS_BITS_TIME_H__ */

