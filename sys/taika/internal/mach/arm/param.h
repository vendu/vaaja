#ifndef __MACH_ARM_PARAM_H__
#define __MACH_ARM_PARAM_H__

#if !defined(__arm64__) && !defined(__aarch64__)
#define MACH_WORD_SIZE      4
#define MACH_CHAR_SIZE      1
#define MACH_SHORT_SIZE     2
#define MACH_INT_SIZE       4
#define MACH_LONG_SIZE      4
#define MACH_LONG_SIZE_LOG2 2
#define MACH_LONG_LONG_SIZE 8
#define MACH_PTR_SIZE       4
#define MACH_PTR_BITS       32
#define MACH_ADR_BITS       32
#define MACH_PAGE_SIZE_LOG2 12
#else
#error arm64 not supported by <zero/param.h> yet
#endif

#define MACH_CL_SIZE        32
#define MACH_CL_SIZE_LOG2   5
#define MACH_PAGE_SIZE      4096

#endif /* __MACH_ARM_PARAM_H__ */

