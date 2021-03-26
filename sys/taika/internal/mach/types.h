#ifndef MACH_TYPES_H
#define MACH_TYPES_H

#if !defined(__STDC__) || (__STDC_VERSION__ < 199901L)
typedef char                    __INT8_T;
typedef unsigned char           __UINT8_T;
typedef short                   __INT816_T;
typedef unsigned short          __UINT16_T;
typedef int                     __INT32_T;
typedef unsigned int            __UINT32_T;
typedef long long               __INT64_T;
typedef unsigned long long      __UINT64_T;
#else
typedef int8_t                  __INT8_T;
typedef uint8_t                 __UINT8_T;
typedef int16_t                 __INT16_T;
typedef uint16_t                __UINT16_T;
typedef int32_t                 __INT32_T;
typedef uint32_t                __UINT32_T;
typedef int64_t                 __INT64_T;
typedef uint64_t                __UINT64_T;
#endif /* defined(__STDC__) && (__STDC_VERSION__ >= 199901L) */

#endif

#endif /* MACH_TYPES_H */

