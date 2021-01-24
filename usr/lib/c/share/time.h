#ifndef SHARE_TIME_H
#define SHARE_TIME_H

/* NOTE: we use long for time_t with 32-bit systems in mind */

#include <share/time_t.h>

#if !defined(__time_types_defined)
typedef long            clock_t;
typedef long            clockid_t;
#if !defined(__timetdefine)
typedef long            time_t;
typedef long            timer_t;
typedef unsigned long   useconds_t;
typedef long            suseconds_t;
#define __time_types_defined 1
#define __time_t_defined     1
#endif

#endif /* SHARE_TIME_H */

