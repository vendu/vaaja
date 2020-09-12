#ifndef __MACH_AMD64_PARAM_H__
#define __MACH_AMD64_PARAM_H__

/* # of I/O ports */
#define MACH_IO_PORTS           65536

#define MACH_WORD_SIZE          8
#define MACH_CHAR_SIZE          1
#define MACH_SHORT_SIZE         2
#define MACH_INT_SIZE           4
#if (defined(_MSC_VER)                                                  \
     || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)       \
     || defined(__CYGWIN__) || defined(__CYGWIN32__)                    \
     || defined(WIN64) || defined(_WIN64)                               \
     || defined(__WIN64) || defined(__WIN64__)                          \
     || defined(__MINGW32__) || defined(__MINGW64__)                    \
     || defined(_M_AMD64)                                              \
     || defined(__ILP32__))
#define MACH_LONG_SIZE          4
#define MACH_LONG_SIZE_LOG2     2
#else
#define MACH_LONG_SIZE          8
#define MACH_LONG_SIZE_LOG2     3
#endif
#define MACH_LONG_LONG_SIZE     8
#define MACH_PTR_SIZE           8
#define MACH_PTR_SIZE_LOG2      3
#define MACH_PTR_BITS           64
#define MACH_ADR_BITS           48 // # of significant bytes in addresses
#define MACH_ADR_HIGH_BITCOPY   1
#undef  MACH_ADR_HIGH_BITMACH
#define MACH_PAGE_SIZE_LOG2     12 // 4-kilobyte pages

#define MACH_CL_SIZE            64
#define MACH_CL_SIZE_LOG2       6
#define MACH_PAGE_SIZE          4096

#define MACH_LONG_DOUBLE_SIZE   80

#endif /* __MACH_AMD64_PARAM_H__ */

