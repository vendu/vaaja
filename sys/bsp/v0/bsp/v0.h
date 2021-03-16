#ifndef V0_BSP_V0_H
#define V0_BSP_V0_H

#if (V0_PLATFORM_TYPE == V0_PAPILI0_PRO)
#include <v0/bsp/papilio-pro.h>
/* FIXME */
#define V0_RAM_SIZE             PAPILIO_PRO_RAM_SIZE
#else
#errof machine-type not specified in <v0/conf.h>
#endif

#define V0_CACHE_LINE_SIZE      32              // cacheline size
#define V0_CACHE_LINE_SHIFT     5               // log2(V0_CACHE_LINE_SIZE)
#define V0_CACHE_STRIDE_SIZE    128             // cache fetch size
#define V0_PAGE_SIZE            (1 << V0_PAGE_SHIFT)
#define V0_HUGE_PAGE_SIZE       (1 << V0_HUGE_PAGE_SHIFT)
#define V0_PAGE_SHIFT           12
#define V0_HUGE_PAGE_SHIFT      16
#define V0_PAGE_TAB_SHIFT       V0_PAGE_SHIFT
#define V0_PAGE_TAB_BITS        10
#define V0_PAGE_TAB_ITEMS       ((1 << V0_PAGE_TAB_BITS) - 1)
#define V0_PAGE_DIR_SHIFT       (V0_PAGE_TAB_SHIFT + V0_PAGE_SHIFT)
#define V0_PAGE_DIR_BITS        (32 - V0_PAGE_TAB_BITS - V0_PAGE_SHIFT)
#define V0_PAGE_DIR_ITEMS       ((1 << V0_PAGE_DIR_BITS) - 1)
#define V0_PAGE_TLB_ENTRIES     (V0_PAGE_SIZE / sizeof(v0pagedesc))
#define V0_IOMAP_ITEMS          (V0_PAGE_SIZE / sizeof(v0iodesc))

#endif /* V0_BSP_V0_H */

