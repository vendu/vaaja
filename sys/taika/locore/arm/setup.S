#include <unit/arm/setup.h>
#include <unit/arm/trap.h>

.globl _start

.extern kloop

.text 32

.org 0x00000000

/* entry point */
_start:
/*
 * interrupt vector table with branch instructions to handler stubs
 * - also entry point at 0x00000000
 * - consists of branch instructions + in-place handler for fast interrupts
 */
_intvec:
	b	_intreset
	b	_intundef
	b	_intswi
	b	_intpref
	b	_intdata
	b	_intadr		// not assigned / emulation
	b	_intirq
/* as last entry, fast interrupt requests get serviced in-place */
_intfiq:
	/* in ARM-mode, saved PC/LR points to last instruction + 8 */
	subs	lr, #4		// adjust link register (return address)
	/* handler code/call here */
	fiqunmask()		// unmask FIQs
	irqunmask()		// unmask IRQs
	bx	lr
	
_intreset:
	/* initialise interrupt stack */
	mov	r1, #TRAPDISAB|TRAPIRQ32
	msr	cpsr_c, r1
	mov	sp, #_intstk
	/* svc stack */
	mov	r1, #TRAPDISAB|TRAPSVC32
	msr	cpsr_c, r1
	mov	sp, #_svcstk
	/* user stack */
	mov	r1, #TRAPUSR32
	msr	cpsr_c, r1
	mov	sp, #_usrstk
	/* enable interrupts */
	fiqunmask()
	irqunmask()
	b	kmain

_intundef:
	/* code here */
	irqunmask()
	bx	lr

_intswi:
	/* code here */
	irqunmask()
	bx	lr

_intpref:
	subs	lr, #4
	/* code here */
	irqunmask()
	bx	lr

_intdata:
	subs	lr, #8
	/* code here */
	irqunmask()
	bx	lr

_intadr:
	/* code here */
	irqunmask()
	bx	lr

_intirq:
	subs	lr, #4
	/* handler call here */
	irqunmask()
	bx	lr

_intstk:	
	.long	TRAPINTSTK
_svcstk:
	.long	TRAPSVCSTK
_usrstk:
	.long	TRAPUSRSTK

