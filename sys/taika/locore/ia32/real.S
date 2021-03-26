#define __ASSEMBLER__ 1
#include <kern/conf.h>

#if (VBE)

#include <kern/unit/x86/boot.h>

.globl	realstart, realend, realint10
.extern	gdtptr, idtptr

.text

.code32

.align	4

	/* get ready for initialising 16-bit real mode */
realint10:
	cli
	pushl	%ebp
	movl	%esp, %ebp
	pushfl					// push EFLAGS
	pushal					// push general registers
	movl	%esp, _espsave			// store stack pointer
	cld
	ljmpl	$REALCODESEL, $BOOTREALBASE	// jump to realstart

.align	4

.code16

	/* drop back to 16-bit real mode */
realstart:
	/* set IDT up at zero */
	lidtl	(_realidtptr - realstart + BOOTREALBASE)
	/* set up 16-bit segments */
	movw	$REALDATASEL, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss
	/* clear protected mode bit */
	mov	%cr0, %eax
	and	$(~CR0PE), %eax
	mov	%eax, %cr0
	/* jump to real mode code */
	ljmpl	$0x0000, $(_real16 - realstart + BOOTREALBASE)

.align	4

	/* generate interrupt 0x10 in real mode */
_real16:
	/* zero segment registers */
	xorw	%ax, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	movw	$(BOOTREALSTK - 16), %sp
	/* load real mode IDT */
	movw	$0x3ff, _realidtptr - realstart + BOOTREALBASE
	lidtl	(_realidtptr - realstart + BOOTREALBASE)
	popa
	sti
	int	$0x10
	cli
	pusha
	lgdtl	(_realgdtptr - realstart + BOOTREALBASE)
	mov	%cr0, %eax
	orl	$(CR0PE), %eax
	mov	%eax, %cr0
	ljmpl	$0x08, $(_protmode - realstart + BOOTREALBASE)

.align	4

.code32

	/* reinitialise 32-bit protected mode */
_protmode:
	lgdtl	(gdtptr)
	/* set up segment registers */
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %ss
	movw	$CPUSEL, %ax
//	movw	%ax, %gs
	movl	_espsave, %esp
	popal
	popfl
	/* leave */
	movl	%ebp, %esp
	popl	%ebp
	ljmpl	$TEXTSEL, $(_protret - realstart + BOOTREALBASE)
_protret:
	lidtl	(idtptr)
	sti
	retl

.align	16

_realgdt:
	/* NULL */
	.short	0, 0
	.byte	0, 0, 0, 0
	/* ring 0 32-bit code descriptor */
	.short	0xffff, 0
	.byte	0, 0x98, 0xcf, 0
	/* ring 0 32-bit data descriptor */
	.short	0xffff, 0
	.byte	0, 0x92, 0xcf, 0

_realgdtptr:
	.short	_realgdtptr - _realgdt - 1
	.long	_realgdt - realstart + BOOTREALBASE

.align	4

_espsave:
	.long	0x00000000

.align	8

_realidtptr:
	.short	0x0000
	.long	0x00000000

.align	4

realend:

#endif /* VBE */

