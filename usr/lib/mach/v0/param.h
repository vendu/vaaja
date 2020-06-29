#ifndef __V0_PARAM_H__
#define __V0_PARAM_H__

#define V0_MACHINE_TYPE         V0_PAPILIO_PRO
#define V0_PAPILIO_PRO          0

#define V0_THREADS              1

/* enable printing with documentation included */
// #define V0_LITERAL_DOCUMENTATION        1

#define MACH_WORD_SIZE          4               // native user register size
#define MACH_CHAR_SIZE          1
#define MACH_SHORT_SIZE         2
#define MACH_INT_SIZE           4
#define MACH_LONG_SIZE          4
#define MACH_LONG_SIZE_LOG2     2

#if (V0_MACHINE_TYPE == V0_PAPILI0_PRO)
#include <v0/bsp/papilio-pro.h>
/* FIXME */
#define V0_RAM_SIZE             PAPILIO_PRO_RAM_SIZE
#else
#error machine-type not specified in <v0/conf.h>
#endif

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
#define MACH_MAX_PROCS          MACH_MAX_THREADS
#define MACH_PROC_THREADS       16                 // max # of process threads
#define MACH_MAX_THREADS        256                // max # of system threads
#define MACH_THR_ID_BITS        8                  // # of bits in thread-IDs
#endif

#define MACH_FB_ADR             0xe0000000         // 3.5 gigabytes

/* optional features */
#define V0_PTHREAD              1                  // POSIX-thread support

#endif /* __V0_PARAM_H__ */

