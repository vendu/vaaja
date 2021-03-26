#define __ASSEMBLER__ 1
#include <kern/conf.h>
#if (APIC)
#include <kern/unit/x86/apic.h>
#endif
#include <kern/unit/x86/pic.h>

/*
 * x86-64 uses 128-byte redzones below the stack pointer; interrupt handlers
 * must not tamper with it... =)
 */
_REDZONESIZE	= 128
#if (APIC)
_APICEOIADR	= APICEOIADR
_APICEOI	= 0x00000000
#endif

/* INTERRUPT MANAGEMENT */

/* caller has saved %rbx, %rbp, %rsi, %rdi, %r12, %r13, %r14, and %r15 */
/* kernel thread-local segment is %gs, user is %fs */	
.macro	_pushregs
	pushq	%rax
	pushq	%rcx
	pushq	%rdx
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11

.macro	_popregs
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rdx
	popq	%rcx
	popq	%rax
	
/* caller has pushed %rip */
.macro	_trapenter
	subq	$_REDZONESIZE, %rsp
	_pushregs
	swapgs
.endm
.macro	_trapleave
	swapgs
	_popregs
.endm
.macro	_trapentererr
	subq	$_REDZONESIZE, %rsp
//	movl	32(%esp), %ecx	// move error code to %ecx
	swapgs
.endm
.macro	_trapleaveerr
	swapgs
//	addl	$4, %esp	// skip error code
.endm

#if (APIC)
.macro	_apiceoi
	/* send APIC EOI (end of interrupt) */
	movl	$_APICEOI, _APICEOIADR
.endm
#endif /* APIC */

#if 0
.macro	_piteoi1
	/* send EOI to PIC1 */
	movb	$PICEOI, %al
	outb	%al, $PIC1
.endm
.macro	_piteoi2
	/* send EOI to PIC2 */
	outb	%al, $PIC2
.endm
#endif


