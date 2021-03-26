#ifndef __UNIT_X86_PIC_H__
#define __UNIT_X86_PIC_H__

#if !defined(__ASSEMBLER__)
#include <sys/io.h>
#include <kern/unit/x86/asm.h>
#endif

/* PIC registers */
#define PICINIT  0x11
#define PIC1     0x20
#define PICMASK1 0x21
#define PIC2     0xa0
#define PICMASK2 0xa1

/* PIC commands */
#define PICEOI   0x20

#define piceoi1() outb(PICEOI, PIC1)
#define piceoi2() outb(PICEOI, PIC2)

#endif /* __UNIT_X86_PIC_H__ */

