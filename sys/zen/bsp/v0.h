#ifndef __ZEN_BSP_V0_H__
#define __ZEN_BSP_V0_H__

#include <stdint.h>
#include <mach/param.h>
#if defined(__v0__)
#include <v0/trap.h>
#endif

/* memory parameters */
#define ZEN_CL_SIZE             32              // memory cacheline size
#define ZEN_STRIDE_SIZE         128             // cache stride size
#define ZEN_PAGE_SIZE           4096            // page size
#define ZEN_JUMBO_SIZE          131072          // jumbo-page size
#define ZEN_BANK_SIZE           2048            // RAM-bank size

/* page-descriptor flags */
#define ZEN_MEM_LOCK            (1 << 0)        // page-entry lock bit
#define ZEN_MEM_CORE            (1 << 1)        // page present in RAM
#define ZEN_MEM_DIRTY     	    (1 << 2)        // memory accessed bit
#define ZEN_MEM_SYSTEM      	(1 << 3)        // system memory
#define ZEN_MEM_GROW_DOWN       (1 << 4)        // stack segments
#define ZEN_MEM_EXEC            (1 << 5)        // executable page
#define ZEN_MEM_WRITE           (1 << 6)        // write permission bit
#define ZEN_MEM_READ            (1 << 7)        // read permission bit
#define ZEN_MEM_NONTEMP   	    (1 << 8)       // non-temporal (uncached) hint
#define ZEN_MEM_FLAG            (1 << 9)       // for system use

/* memory layout */

#define ZEN_TRAP_VECTOR         0x00000000
#define ZEN_SYS_STACK_SIZE      ZEN_PAGE_SIZE
#define ZEN_USR_STACK_SIZE      (4 * ZEN_PAGE_SIZE)
#define ZEN_TEXT_MEM            ZEN_PAGE_SIZE
#define ZEN_USR_STACK_LIM       (ZEN_USR_STACK - ZEN_USR_STACK_SIZE)
#define ZEN_USR_STACK           (ZEN_SYS_STACK - ZEN_SYS_STACK_SIZE)
#define ZEN_SYS_STACK_LIM       (ZEN_SYS_STACK - ZEN_SYS_STACK_SIZE)
#define ZEN_SYS_STACK           ZEN_RAM_SIZE

#endif /* __ZEN_BSP_ZEN_H__ */

