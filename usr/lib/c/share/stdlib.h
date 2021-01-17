#ifndef SHARE_STDLIB_H
#define SHARE_STDLIB_H

#if defined(_ZERO_SOURCE)
#include <mach/param.h>
#include <stdint.h>
#if (MACH_PTR_SIZE == 8)
typedef int64_t                 ptrdiff_t;
#elif (MACH_PTR_SIZE == 4)
#endif
#elif defined(_MSC_VER) && defined(_WIN64)
typedef long long               ptrdiff_t;
#else
typedef long                    ptrdiff_t;
#endif
#define __ptrdiff_t_defined     1
#if !defined(NULL)
#include <share/null.h>
#endif
#if !defined(__size_t_defined)
#include <share/size.h>
#endif
#if !defined(__wchar_t_defined)
#include <share/wchar.h>
#endif

#endif /* __SHARE_STDLIB_H__ */

