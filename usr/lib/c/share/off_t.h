#ifndef SHARE_OFF_T_H
#define SHARE_OFF_T_H

#include <mach/param.h>
#include <stdint.h>
#include <share/null.h>
#if defined(_MSC_VER) && defined(_WIN64)
typedef long long               off_t;
#elif defined(MACH_LONG_LONG_SIZE)
typedef long long               off_t;
#elif (MACH_WORD_SIZE == 4)
typedef int32_t                 off_t;
#else
typedef long                    off_t;
#endif
#define __off_t_defined         1

#endif /* SHARE_OFF_T_H */

