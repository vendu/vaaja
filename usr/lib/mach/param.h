#ifndef __MACH_PARAM_H__
#define __MACH_PARAM_H__

/*
 * machine-specific param.h headers should declare the following:
 * MACH_WORD_SIZE       - sizeof machine word/register in bytes
 * MACH_CHAR_SIZE       - sizeof(char) in bytes
 * MACH_SHORT_SIZE      - sizeof(short) in bytes
 * MACH_INT_SIZE        - sizeof(int) in bytes
 * MACH_LONG_SIZE       - sizeof(long) in bytes
 * MACH_LONG_LONG_SIZE  - sizeof(long long) in bytes
 * MACH_PTR_SIZE        - sizeof pointer in bytes
 * MACH_PTR_BITS        - number of bits in pointers
 * MACH_ADR_BITS        - number of [low] significant bits in memory addresses
 * MACH_CL_SIZE         - cacheline size
 * MACH_PAGE_SIZE       - virtual memory page size
 */
#if defined(__v0__)
#include <mach/v0/param.h>
#elif defined(__x86_64__) || defined(__amd64__)
#include <mach/x86-64/param.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <mach/ia32/param.h>
#elif defined(__arm__)
#include <mach/arm/param.h>
#elif defined(__ppc__)
#include <mach/ppc/param.h>
#endif

#endif /* __MACH_PARAM_H__ */

