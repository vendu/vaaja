#ifndef MACH_V0_MEM_H
#define MACH_V0_MEM_H

#define MEM_EXEC      (1 << 0)
#define MEM_WRITE     (1 << 1)
#define MEM_READ      (1 << 2)
#define MEM_WIRE      (1 << 3)

#define MEM_PRESENT   (1 << 4)
#define MEM_DIRTY     (1 << 5)
#define MEM_SYS       (1 << 6)
#define MEM_GROW_DOWN (1 << 7)
#define MEM_DYNAMIC   (1 << 8)

#endif /* MACH_V0_MEM_H */
