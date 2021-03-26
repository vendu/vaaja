#ifndef __UNIT_X86_64_SEG_H__
#define __UNIT_X86_64_SEG_H__

#include <stdint.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/x86/seg.h>

#define SEGDEFBITS (SEG64BIT | SEG4KGRAN | SEGPRES)
#define SEGTSS     (SEGAVAILTSS | SEGUSER | SEGPRES | SEGISTMASK)
#define SEG64BIT   UINT64_C(0x0020000000000000)

#endif /* __UNIT_X86_64_SEG_H__ */

