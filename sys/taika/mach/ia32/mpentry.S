/*
 * this code relies on the caller to wait for the CPU to be marked active
 * - it's not reentrant
 *
 * mpentry is located at 0x9f000
 * GDT is located at MPGDT
 * call stack is located at MPENTRYSTK
 * processor kernel stack is located at MPSTKSIZE + cpu->apicid * MPSTKSIZE
 *
 * call stack
 * ----------
 *
 * 0x9e000	- stack top
 * 0x9dffc	- APIC ID
 * 0x9dff8	- processor kernel stack address
 */

#define __ASSEMBLER__ 1
#include <kern/conf.h>

#if (SMP)

#include <kern/unit/x86/boot.h>
#include <kern/unit/ia32/mp.h>

.globl	mpentry, mpend

.text	16

mpentry:
	/* disable interrupts */
	cli
//	ljmpl	$MPENTRYSEG, (_mpflush - mpentry + MPENTRY)
//_mpflush:
	/* set data segment */
	movw	$(MPENTRY >> 4), %ax
	movw	%ax, %ds
	/* zero the rest of segment registers */
	xorw	%ax, %ax
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	lgdtl	(_mpgdtptr - mpentry + MPENTRY)
	/* switch to protected mode */
	movl	%cr0, %eax
	orl	$CR0PE, %eax
	movl	%eax, %cr0
	ljmpl	$TEXTSEL, $(_mpentry32 - mpentry + MPENTRY)

.text	32
_mpentry32:
	/* load other segment registers */
	movw	$DATASEL, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	/* set the stack up */
	movw	%ax, %ss
	movl	$(MPENTRYSTK - 8), %esp
#if 0
	movl	$(MPENTRYSTK - 12), %esp
	/* load PDBR and enable paging */
	popl	%eax			// page directory address
	movl	%eax, %cr3
	movl	%cr0, %eax
	orl	$CR0PG, %eax
	movl	%eax, %cr0
#endif
	popl	%eax			// kernel stack address
//	popl	%ebx			// APIC ID
	/* initialise processor kernel stack */
	movl	%eax, %esp
	/* the stack has the APIC ID as an argument for mpmain */
//	pushl	%ebx
	call	mpmain

.align	16

_mpgdt:
	/* NULL */
	.short	0, 0
	.byte	0, 0, 0, 0
	/* ring 0 32-bit code descriptor */
	.short	0xffff, 0
	.byte	0, 0x98, 0xcf, 0
	/* ring 0 32-bit data descriptor */
	.short	0xffff, 0
	.byte	0, 0x92, 0xcf, 0

_mpgdtptr:
	.short	_mpgdtptr - _mpgdt - 1
	.long	_mpgdt - mpentry + MPENTRY

.align	4
	
mpend:
	
#endif /* SMP */

