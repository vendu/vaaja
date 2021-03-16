#ifndef MACH_V0_MEM_H
#define MACH_V0_MEM_H

/* SUPPORT FOR V0 ARCHITECTURE */

/* page-descriptor flags */
#define MACH_MEM_EXEC           (1 << 0)    // execute permission
#define MACH_MEM_WRITE          (1 << 1)    // write permission
#define MACH_MEM_READ           (1 << 2)    // read permission
#define MACH_MEM_PRESENT        (1 << 3)    // page is present in RAM
#define MACH_MEM_DIRTY          (1 << 4)    // page has been written into
#define MACH_MEM_WIRE           (1 << 5)    // mapped to RAM permanently
#define MACH_MEM_SYSTEM         (1 << 6)    // system-page (not user)
#define MACH_MEM_NONTEMP        (1 << 7)    // non-temporary [uncached] hint
/* page-allocator flags */
#define VM_SEG_DYNAMIC          (1 << 8)    // dynamically-allocated segment
#define VM_SEG_GROWDOWN         (1 << 9)    // grow-down bit for stack segments
#define VM_SEG_ZERO             (1 << 10)   // zero on allocation
/* flags for process segments */
#define VM_SEG_NULL             0
#define VM_SEG_TRAP             VM_MEM_READ
#define VM_SEG_TEXT             (VM_MEM_READ | VM_MEM_EXEC)
#define VM_SEG_RODATA           VM_MEM_READ
#define VM_SEG_DATA             (VM_MEM_READ | VM_MEM_WRITE)
#define VM_SEG_BSS              (VM_MEM_DATA | VM_MEM_ZERO | VM_SEG_DYNAMIC)
#define VM_SEG_COMM             VM_MEM_RODATA
#define VM_SEG_DEBUG            VM_MEM_RODATA
#define VM_SEG_STACK            (VM_MEM_READ | VM_MEM_WRITE | VM_SEG_GROWDOWN)
#define VM_SYS_SEG(seg)         ((seg) | VM_MEM_SYSTEM)
/* segment IDs */
#define VM_NULL_SEG             0
#define VM_TRAP_SEG             1
#define VM_TEXT_SEG             2
#define VM_RODATA_SEG           3
#define VM_DATA_SEG             4
#define VM_BSS_SEG              5
#define VM_COMM_SEG             6
#define VM_DEBUG_SEG            7
#define VM_STACK_SEG            8
#define VM_TASK_SEGS            10
/* not to be mixed with user-space segments (there may be extra ones) */

#endif /* MACH_V0_MEM_H */
