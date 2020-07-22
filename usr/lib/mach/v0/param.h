#ifndef __V0_PARAM_H__
#define __V0_PARAM_H__

/* enable printing with documentation included */
// #define V0_LITERAL_DOCUMENTATION        1

#define MACH_WORD_SIZE          4               // native user register size
#define MACH_CHAR_SIZE          1
#define MACH_SHORT_SIZE         2
#define MACH_INT_SIZE           4
#define MACH_LONG_SIZE          4
#define MACH_LONG_SIZE_LOG2     2

#define MACH_CL_SIZE            32              // cacheline size
#define MACH_CL_SHIFT           5               // log2(V0_CACHE_LINE_SIZE)
#define MACH_CACHE_STRIDE_SIZE  128             // cache fetch size
#define MACH_PAGE_SIZE          (1 << MACH_PAGE_SHIFT)
#define MACH_HUGE_PAGE_SIZE     (1 << MACH_HUGE_PAGE_SHIFT)
#define MACH_PAGE_SHIFT         12
#define MACH_HUGE_PAGE_SHIFT    16
#define MACH_PAGE_TAB_SHIFT     MACH_PAGE_SHIFT
#define MACH_PAGE_TAB_BITS      10
#define MACH_PAGE_TAB_ITEMS     ((1 << MACH_PAGE_TAB_BITS) - 1)
#define MACH_PAGE_DIR_SHIFT     (MACH_PAGE_TAB_SHIFT + MACH_PAGE_SHIFT)
#define MACH_PAGE_DIR_BITS      (32 - MACH_PAGE_TAB_BITS - MACH_PAGE_SHIFT)
#define MACH_PAGE_DIR_ITEMS     ((1 << MACH_PAGE_DIR_BITS) - 1)
#define MACH_PAGE_TLB_ENTRIES   (MACH_PAGE_SIZE / sizeof(v0pagedesc))
#define MACH_IOMAP_ITEMS        (MACH_PAGE_SIZE / sizeof(v0iodesc))

#endif /* __V0_PARAM_H__ */

