#ifndef MACH_V0_MEM_H
#define MACH_V0_MEM_H

/* SUPPORT FOR V0 ARCHITECTURE */

/* page-descriptor flags */
#define V0_MEM_EXEC     (1 << 0)    // execute permission
#define V0_MEM_WRITE    (1 << 1)    // write permission
#define V0_MEM_READ     (1 << 2)    // read permission
#define V0_MEM_PRESENT  (1 << 3)    // page is present in RAM
#define V0_MEM_DIRTY    (1 << 4)    // page has been written into
#define V0_MEM_WIRE     (1 << 5)    // page is mapped to RAM permanently
#define V0_MEM_SYSTEM   (1 << 6)    // system-page (as opposed to user-mode)
#define V0_MEM_NONTEMP  (1 << 7)    // non-temporary [uncached] hint
/* page-allocator flags */
#define V0_SEG_DYNAMIC  (1 << 8)    // dynamically-allocated segment
#define V0_SEG_GROWDOWN (1 << 9)    // grow-down bit for stack segments
#define V0_SEG_ZERO     (1 << 10)   // zero on allocation
/* flags for process segments */
#define V0_SEG_TRAP     V0_MEM_READ
#define V0_SEG_RODATA   V0_MEM_READ
#define V0_SEG_TEXT     (V0_MEM_READ | V0_MEM_EXEC)
#define V0_SEG_DATA     (V0_MEM_READ | V0_MEM_WRITE)
#define V0_SEG_BSS      (V0_MEM_DATA | V0_MEM_ZERO | V0_SEG_DYNAMIC)
#define V0_SEG_STACK    (V0_MEM_READ | V0_MEM_WRITE | V0_SEG_GROWDOWN)
#define V0_SEG_COMM     V0_MEM_RODATA
#define V0_SEG_DEBUG    V0_MEM_RODATA
#define V0_SYS_SEG(seg) ((seg) | V0_MEM_SYSTEM)
/* segment IDs */
#define V0_TRAP_SEG     0
#define V0_RODATA_SEG   1
#define V0_TEXT_SEG     2
#define V0_DATA_SEG     3
#define V0_BSS_SEG      4
#define V0_COMM_SEG     5
#define V0_DEBUG_SEG    6
#define V0_CUSTOM_SEG   7
#define V0_SEGS         8

#endif /* MACH_V0_MEM_H */
