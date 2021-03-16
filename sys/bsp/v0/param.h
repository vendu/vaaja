#ifndef V0_PARAM_H
#define V0_PARAM_H

#define MACH_WORD_SIZE          4
#define MACH_CHAR_SIZE          1
#define MACH_SHORT_SIZE         2
#define MACH_INT_SIZE           4
#define MACH_LONG_SIZE          4
#define MACH_LONG_LONG_SIZE     8
#define MACH_PTR_SIZE           4
#define MACH_PTR_BITS           32
#define MACH_ADR_BITS           32
#define MACH_CL_SIZE            32
#define MACH_PAGE_SIZE          4096

#define MACH_WORD_SIZE_LOG2     2
#define MACH_LONG_SIZE_LOG2     2

/* memory parameters */
#define MACH_CL_SIZE            32              // memory cacheline size
#define MACH_STRIDE_SIZE        128             // cache stride size
#define MACH_PAGE_SIZE          4096            // page size
//#define MACH_JUMBO_SIZE         32768           // jumbo-page size
#define MACH_BANK_SIZE          2048            // RAM-bank size

#define MACH_PAGE_DIR_SIZE      1024
#define MACH_PAGE_TAB_SIZE      1024

#endif /* V0_PARAM_H */

