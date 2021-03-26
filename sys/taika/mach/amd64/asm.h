#ifndef __KERN_UNIT_X86_64_ASM_H__
#define __KERN_UNIT_X86_64_ASM_H__

#define FSMSR  0xc0000100       // user thread-local segment
#define GSMSR  0xc0000101       // user-mode thread-local segment
#define KGSMSR 0xc0000102       // kernel-mode thread-local segment

#endif /* __KERN_UNIT_X86_64_ASM_H__ */

