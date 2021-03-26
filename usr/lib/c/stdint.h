#ifndef __STDINT_H__
#define __STDINT_H__

#include <internal/mach/param.h>

#define INT8_C(c)    c
#define INT16_C(s)   s
#define INT32_C(i)   i
#define UINT8_C(uc)  uc##U
#define UINT16_C(us) us##U
#define UINT32_C(ui) ui##U
#if defined(_MSC_VER)
#define INT64_C(l)   l##i64
#define UINT64_C(ul) ul##ui64
#elif (MACH_WORD_SIZE == 4)
#define INT64_C(l)   l##LL
#define UINT64_C(ul) ul##ULL
#elif (MACH_WORD_SIZE == 8)
#define INT64_C(l)   l##L
#define UINT64_C(ul) ul##UL
#endif

/* explicit-size types */
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
#if (MACH_WORD_SIZE == 4) || defined(_MSC_VER)
typedef long long          int64_t;
typedef unsigned long long uint64_t;
#elif (MACH_WORD_SIZE == 8)
typedef long               int64_t;
typedef unsigned long      uint64_t;
#endif

#if defined(__x86_64__) || defined(__amd64__) || defined(__i386__)
/* fast types */
typedef char               int_fast8_t;
typedef unsigned char      uint_fast8_t;
typedef short              int_fast16_t;
typedef unsigned short     uint_fast16_t;
typedef int                int_fast32_t;
typedef unsigned int       uint_fast32_t;
#if (MACH_WORD_SIZE == 4) || defined(_MSC_VER)
typedef long long          int_fast64_t;
typedef unsigned long long uint_fast64_t;
#elif (MACH_WORD_SIZE == 8)
typedef long               int_fast64_t;
typedef unsigned long      uint_fast64_t;
#else
#error declare fast types for the platform in <stdint.h>
#endif

/* least-width types */
typedef char               int_least8_t;
typedef unsigned char      uint_least8_t;
typedef short              int_least16_t;
typedef unsigned short     uint_least16_t;
typedef int                int_least32_t;
typedef unsigned int       uint_least32_t;
#if (MACH_WORD_SIZE == 4) || defined(_MSC_VER)
typedef long long          int_least64_t;
typedef unsigned long long uint_least64_t;
#elif (MACH_WORD_SIZE == 8)
typedef long               int_least64_t;
typedef unsigned long      uint_least64_t;
#else
#error declare least-width types for the platform in <stdint.h>
#endif

/* maximum-size integral types */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef long long          intmax_t;
typedef unsigned long long uintmax_t;
#else /* before C99 */
typedef long               intmax_t;
typedef unsigned long      uintmax_t;
#endif /* __STDC_VERSION__ */

/* pointer integer types */
#if (MACH_ADR_BITS <= 32)
typedef int32_t            intptr_t;
typedef uint32_t           uintptr_t;
#else
typedef int64_t            intptr_t;
typedef uint64_t           uintptr_t;
#endif

/* limits for explicit-size types */
#define INT8_MIN         (-0x7f - 1)
#define INT8_MAX         0x7f
#define UINT8_MAX        0xff
#define INT16_MIN        (-0x7fff - 1)
#define INT16_MAX        0x7fff
#define UINT16_MAX       0xffff
#define INT32_MIN        (-0x7fffffff - 1)
#define INT32_MAX        0x7fffffff
#define UINT32_MAX       0xffffffff
#define INT64_MIN        INT64_C(-0x7fffffffffffffff - 1)
#define INT64_MAX        INT64_C(0x7fffffffffffffff)
#define UINT64_MAX       UINT64_C(0xffffffffffffffff)
/* limits for fast types */
#define INT_FAST8_MIN    (-0x7f - 1)
#define INT_FAST8_MAX    0x7f
#define UINT_FAST8_MAX   0xff
#define INT_FAST16_MIN   (-0x7fff - 1)
#define INT_FAST16_MAX   0x7fff
#define UINT_FAST16_MAX  0xffff
#define INT_FAST32_MIN   (-0x7fffffff - 1)
#define INT_FAST32_MAX   0x7fffffff
#define UINT_FAST32_MAX  0xffffffff
#define INT_FAST64_MIN   INT64_C(-0x7fffffffffffffff - 1)
#define INT_FAST64_MAX   INT64_C(0x7fffffffffffffff)
#define UINT_FAST64_MAX  UINT64_C(0xffffffffffffffff)
/* limits for least-size types */
#define INT_LEAST8_MIN   (-0x7f - 1)
#define INT_LEAST8_MAX   0x7f
#define UINT_LEAST8_MAX  0xff
#define INT_LEAST16_MIN  (-0x7fff - 1)
#define INT_LEAST16_MAX  0x7fff
#define UINT_LEAST16_MAX 0xffff
#define INT_LEAST32_MIN  (-0x7fffffff - 1)
#define INT_LEAST32_MAX  0x7fffffff
#define UINT_LEAST32_MAX 0xffffffff
#define INT_LEAST64_MIN  INT64_C(-0x7fffffffffffffff - 1)
#define INT_LEAST64_MAX  INT64_C(0x7fffffffffffffff)
#define UINT_LEAST64_MAX UINT64_C(0xffffffffffffffff)
/* other integer type limits */
#define PTRDIFF_MIN      (~(ptrdiff_t)0 << (sizeof(ptrdiff_t) * CHAR_BIT - 1))
#define PTRDIFF_MAX      (~PTRDIFF_MIN)
#define SIG_ATOMIC_MIN   0
#define SIG_ATOMIC_MAX   (~0L)
#define WCHAR_MIN        ((wchar_t)0)
#define WCHAR_MAX        (~(wchar_t)0)
#define WINT_MIN         ((wint_t)0)
#define WINT_MAX         (~(wint_t)0)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define SIZE_MAX         (~(size_t)0)
#endif

#endif /* __x86_64_ || __amd64__ || __i386__ */

#endif /* __STDINT_H__ */

