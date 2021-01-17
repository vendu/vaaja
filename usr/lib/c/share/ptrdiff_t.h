#ifndef SHARE_PTRDIFF_T_H
#define SHARE_PTRDIFF_T_H

#if defined(_ZERO_SOURCE)
#include <mach/param.h>
#include <stdint.h>
#include <share/null.h>
#if defined(_MSC_VER) && defined(_WIN64)
typedef long long          ptrdiff_t;
#elif (MACH_PTR_SIZE == 8)
typedef int64_t            ptrdiff_t;
#elif (MACH_PTR_SIZE == 4)
typedef int32_t            ptrdiff_t;
#endif
#else
typedef long               ptrdiff_t;
#endif
#define __ptrdiff_t_defined 1

#endif /* SHARE_PTRDIFF_T_H */

