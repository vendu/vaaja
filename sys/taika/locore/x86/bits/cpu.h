#ifndef __KERN_UNIT_X86_BITS_CPU_H__
#define __KERN_UNIT_X86_BITS_CPU_H__

#define CPUFPUCTXSIZE 512               // arrange this much room for context

#define CPUHASFXSR    0x00000001
#define CPUUSEDFPU    0x00000002
#define CPUHASAPIC    0x00000004
#define CPU_CR4_TSD   (1 << 2)
#define CPU_CR4_DE    (1 << 3)
#define CPU_CR4_PSE   (1 << 4)
#define CPU_CR4_PGE   (1 << 7)
#define CPU_CR4_PCE   (1 << 8)

#endif /* __KERN_UNIT_X86_BITS_CPU_H__ */

