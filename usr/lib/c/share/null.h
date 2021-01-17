#ifndef SHARE_NULL_H
#define SHARE_NULL_H

#include <stdint.h>
#include <mach/param.h>

#if !defined(NULL)
#if defined(_MSC_VER) && defined(_WIN64)
#define NULL ((void *)0ULL)
#elif (MACH_PTR_SIZE == 8)
#define NULL ((void *)UINT64_C(0))
#elif (MACH_PTR_SIZE == 4)
#define NULL ((void *)UINT32_C(0))
#else
#define NULL ((void *)0UL))
#endif
#endif

#endif /* SHARE_NULL_H */

