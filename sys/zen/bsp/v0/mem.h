#ifndef __SYS_ZEN_BSP_V0_MEM_H__
#define __SYS_ZEN_BSP_V0_MEM_H__

/* SUPPORT FOR V0 ARCHITECTURE */

/* page-descriptor flags */
#define V0_SEG_READ     (1 << 0)    // read permission
#define V0_SEG_WRITE    (1 << 1)    // write permission
#define V0_SEG_EXEC     (1 << 2)    // execute permission
#define V0_SEG_SYSTEM   (1 << 3)    // system-segment (as opposed to user-mode)
#define V0_SEG_DIRTY    (1 << 4)    // segment has been written into
#define V0_SEG_CORE     (1 << 5)    // segment is present in RAM
#define V0_SEG_GROWDOWN (1 << 6)    // grow-down bit for stack segmentws
#define V0_SEG_NONTEMP  (1 << 7)    // non-temporary [uncached] hint
#define V0_SEG_ZERO     (1 << 8)    // initiliaze to zero
#define V0_SEG_INIT     (1 << 9)    // segment is valid/initialized
#define V0_SEG_LCOK_BIT (1 << 10)   // descriptor lock-bit for page-table updates
/* flags for process segments */
#define V0_SEG_TRAP     V0_SEG_READ
#define V0_SEG_RODATA   V0_SEG_READ
#define V0_SEG_TEXT     (V0_SEG_READ | V0_SEG_EXEC)
#define V0_SEG_DATA     (V0_SEG_READ | V0_SEG_WRITE | V0_SEG_INIT)
#define V0_SEG_BSS      (V0_SEG_DATA | V0_SEG_ZERO | V0_SEG_ALLOC)
#define V0_SEG_STACK    (V0_SEG_READ | V0_SEG_WRITE | V0_GROWDOWN)
#define V0_SEG_COMM     V0_SEG_RODATA
#define V0_SEG_DEBUG    V0_SEG_RODATA
#define V0_SEG_ALLOC    (1U << 11) // allocate output section
#define V0_SYS_SEG(seg) ((seg) | V0_SEG_SYSTEM)
/* segment IDs */
#define V0_TRAP_SEG   0
#define V0_RODATA_SEG 1
#define V0_TEXT_SEG   2
#define V0_DATA_SEG   3
#define V0_BSS_SEG    4
#define V0_COMM_SEG   5
#define V0_DEBUG_SEG  6
#define V0_SEGS       8

#endif /* #ifndef __SYS_ZEN_BSP_V0_MEM_H__ */

