#ifndef SHARE_OFF_T_H
#define SHARE_OFF_T_H

#include <mach/param.h>

#if defined(_MSC_VER) && defined(_WIN64)
typedef long long               off_t;
#elif defined(MACH_LONG_LONG_SIZE)
typedef long long               off_t;
#elif defined(__v0__)
typedef int32_t

#else
typedef long                    off_t;
#endif
#define __off_t_defined         1

#endif /* SHARE_OFF_T_H */

