#ifndef __V0_CONF_H__
#define __V0_CONF_H__

#define V0_MACHINE_TYPE         V0_PAPILIO_PRO
#define V0_PAPILIO_PRO          0

#define V0_THREADS              1

/* enable printing with documentation included */
// #define V0_LITERAL_DOCUMENTATION        1

#if (V0_MACHINE_TYPE == V0_PAPILI0_PRO)
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

#define V0_SERVER_CONF          0
#define V0_DESKTOP_CONF         1
#define V0_LOW_LATENCY_CONF     2

#if (V0_LOW_LATENCY_DESKTOP)
#define V0_TMR_HZ               128
#define V0_TMR_SLICE_MS         4
#elif (V0_DESKTOP_CONF)
#define V0_TMR_HZ               64
#define V0_TMR_SLICE_MS         64
#elif (V0_SERVER_CONF
#define V0_TMR_HZ               16
#define V0_TMR_SLICE            32
#else
#errof host-type not specified in <v0/conf.h>
#endif
#define V0_WORD_SIZE            4               // native user register size
#define V0_MAX_UNITS            16              // max # of unit IDs
#define V0_MAX_INSTS            64              // max # of unit instruction IDs
#define _MEGABYTE               (1024U * 1024U)
#define V0_INTR_VECTOR          0x00000000      // system interrupt vector
#define V0_CODE_ADR             V0_PAGE_SIZE    // system text segment address
#define V0_CODE_PROT_SIZE       (3 * V0_PAGE_SIZE) // protector maximum size
#define V0_SYS_STACK            0x00000000         // system stack top
#define V0_STACK_SIZE           (3 * V0_PAGE_SIZE) // per-thread stack size
#define V0_STACK_PROT_SIZE      V0_PAGE_SIZE       // protector maximum size
#if defined(V0_THREADS)
#define V0_PROC_THREADS         16                 // max # of process threads
#define V0_MAX_THREADS          256                // max # of system threads
#define V0_THR_ID_BITS          8                  // # of bits in thread-IDs
#endif

#define V0_FB_ADR               0xe0000000         // 3.5 gigabytes

/* optional features */
#define V0_PTHREAD              1                  // POSIX-thread support

#endif /* __V0_CONF_H__ */

