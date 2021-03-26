#ifndef TAIKA_INTERNAL_TYPES_H
#define TAIKA_INTERNAL_TYPES_H

#include <sys/taika/conf.h>

#if defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)
#define __OFF_T                 __INT64_T
#define __SSIZE_T               __INT64_T
#else
#define __OFF64_T               __INT64_T
#define __OFF_T                 __INT32_T
#define __SSIZE_T               __INT32_T
#endif
#if defined(MACH_PTR_BITS) && (MACH_PTR_BITS == 64)
#define __SIZE_T                __INT64_T
#else
#define __SIZE_T                __INT32_T
#endif
#if defined(TAIKA_TIME_BITS) && (TAIKA_TIME_BITS == 64)
#define __TIME_T                __INT64_T
#else
#define __TIME_T                __INT32_T
#endif

#define __PID_T                 __INT32_T
#define __UID_T                 __UINT32_T
#define __GID_T                 __UINT32_T
#define __FD_T                  __INT32_T
#define __BLKCNT_T              __UINT32_T
#define __BLKSIZE_T             __UINT32_T
#define __CLOCK_T               __INT32_T
#define __CLOCKID_T             __INT32_T
#define __DEV_T                 __INT32_T
#define __FSBLKCNT_T            __INT32_T
#define __FSFILCNT_T            __INT32_T
#define __ID_T                  __INT32_T
#define __INO_T                 __INT32_T
#define __MODE_T                __UINT32_T
#define __NLINK_T               __UINT32_T
#define __USECONDS_T            __UINT32_T
#define __SUSECONDS_T           __INT32_T
#define __KEY_T                 __INT32_T

#endif /* TAIKA_INTERNAL_TYPES_H */

