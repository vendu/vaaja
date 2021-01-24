#ifndef SHARE_TIME_T_H
#define SHARE_TIME_T_H

#include <mach/param.h>

#if defined(_MSC_VER) && defined(_WIN64)
typedef long long               time_t;
#elif defined(MACH_LONG_LONG_SIZE)
typedef long long               time_t;
#elif defined(__v0__)
typedef int32_t                 time_t;
#else
typedef long                    time_t;
#endif
#define __time_t_defined        1

#endif /* SHARE_TIME_T_H */

