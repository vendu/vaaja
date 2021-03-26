#define __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/boot.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/x86/bits/cpu.h>
#include <kern/unit/ia32/task.h>
#include "macro.S"

/*
 * NOTE: we use interrupt gates, which automatically disable further
 * interrupts.
 */

/*
 * FIXME
 * - make the interrupt handlers add to interrupt mask, not set it
 * - use local APIC instead of PIC + PIT
 */

/*
 * CPU pushes EFLAGS, CS, and EIP, as well as an ERROR-code word if used.
 * return with IRET after return from handler.
 *
 * top of stack after interrupt
 *
 * eflags
 * cs
 * eip		<- ESP if error code not present
 * (err)	<- ESP if error code supplied
 */

/*
 * interrupt prologue
 * ------------------
 * ESP points to IRET stack frame OR ERROR-code right below it.
 * push general-purpose registers with PUSHA.
 * possible ERROR-code is stored EDX.
 */

/*
 * interrupt epilogue
 * ------------------
 * interrupt routines with error must adjust stack pointer.
 * in the end, we enable interrupts with STI.
 */

.extern	pittimerticks

.globl	trapde, trapdb, trapnmi, trapbp, trapof, trapbr, trapud, trapnm, trapdf
.globl	trapts, trapnp, trapss, trapgp, trappf, trapmf, trapac, trapmc, trapxf
.globl	irqtmr0, irqtmr, irqkbd, irqmouse, irqerror, irqspurious
.globl	irqvec
#if (APIC) || (SMP)
.globl	irqerror, irqspurious, apicspurint, irqtmrcnt
#endif

.extern	sigfunc, tmrsyncscr, schedswitchtask
.extern vmpagefault, kbdtrap, mousetrap
#if (APIC)
.extern	mpapic
#endif

.text	32

trapde:
	pushl	$0
	pushl	$TRAPDE
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPDE, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapdb:
	pushl	$0
	pushl	$TRAPDB
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPDB, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapnmi:
	pushl	$0
	pushl	$TRAPNMI
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPNMI, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapbp:
	pushl	$0
	pushl	$TRAPBP
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPBP, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapof:
	pushl	$0
	pushl	$TRAPOF
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPOF, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapbr:
	pushl	$0
	pushl	$TRAPBR
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPBR, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapud:
	pushl	$0
	pushl	$TRAPUD
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %eax	// old ESP
	movl	(%eax), %ecx	// stored EIP
	pushl	%eax
	movl	$TRAPUD, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapnm:
	pushl	$0
	pushl	$TRAPNM
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPNM, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapdf:
	pushl	$TRAPDF
	_trapentererr
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx			// push context/frame argument
	movl	$TRAPDF, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapts:
	pushl	$TRAPTS
	_trapentererr
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx			// push context/frame argument
	movl	$TRAPTS, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

	/* error code is segment selector index */
trapnp:
	pushl	$TRAPNP
	_trapentererr
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx			// push context/frame argument
	movl	$TRAPNP, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

	/* error code is segment selector index if not present, 0 otherwise */
trapss:
	pushl	$TRAPSS
	_trapentererr
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx			// push context/frame argument
	movl	$TRAPSS, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

	/* error-code is segment selector index if segment-related or 0 */
trapgp:
	pushl	$TRAPGP
	_trapentererr
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx
	movl	$TRAPGP, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
_gpfin:
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

	/*
	 * - cr2 has virtual fault address
	 * - bit 0 of error 1 if protection violation, 0 if non-present page
	 * - bit 1 of error 1 if write, 0 if read
	 * - bit 2 of error 1 if CPL was 3
	 * - bit 3 of error 1 if 1 present in a reserved field
	 * - bit 4 set if caused by an instruction fetch
	 */
trappf:
	pushl	$TRAPPF
	_trapentererr
	movl	%esp, %ebp
	leal	40(%esp), %ebx		// stack frame argument on stack
	/* error code is in %ecx */
	pushl	%ebx
	movl	%cr2, %edx		// fault address in edx
	xorl	%gs:12, %eax		// FIXME: PID
	call	vmpagefault
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapmf:
	pushl	$0
	pushl	$TRAPMF
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPMF, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapac:
	pushl	$0
	pushl	$TRAPAC
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPAC, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapmc:
	pushl	$0
	pushl	$TRAPMC
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPMC, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

trapxf:
	pushl	$0
	pushl	$TRAPXF
	_trapenter
	movl	%esp, %ebp
	movl	8(%esp), %ebx		// stack frame argument on stack
	xorl	%ecx, %ecx		// zero third argument
	pushl	%ebx			// push context/frame argument
	movl	$TRAPXF, %edx
	xorl	%eax, %eax
	pushl	%esp			// ctx-argument for sigfunc
	call	sigfunc
	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

#if (APIC) || (SMP)
	/* dummy interrupt handler for probing APIC frequency */
irqtmrcnt:
	_apiceoi
	sti
	iretl
#endif

	/*
	 * simple interface to let the PIT do sleep() and such actions early in
	 * the bootstrap process
	 */
irqtmr0:
	/* push general purpose registers */
	_trapenter
//	pushal
	/* store stack pointer */
	movl	%esp, %ebp
	/* load interrupt vector address */
	movl	$irqvec, %eax
	/* load interrupt handler address */
	movl	0(%eax), %ebx
	/* call if non-zero */
	cmpl	$0, %ebx
	jz		_tmr0fin
	call	*%ebx
	/* zero interrupt handler address */
	movl	$irqvec, %eax
	movl	$0x00000000, 0(%eax)

_tmr0fin:
	_piceoi1
	movl	%ebp, %esp
	/* pop general purpose registers */
	popal
	/* re-enable interrupts */
	sti
	/* return */
	iretl

	/* 32-bit x86 context switch */
.align	16
	/*
	 * registers after interrupt
	 * -------------------------
	 * ebp		- old ESP _and_ EBP
	 *
	 * top of stack after prologue
	 * ---------------------------
	 * ss		- user-stack segment selector
	 * esp		- userland stack pointer
	 * eflags
	 * cs
	 * eip
	 * ebp		<- EBP, ESP
	 * eax
	 *
	 * NOTE: ss and esp are only pushed if the interrupt happened
	 * in user-mode, i.e. there's a privilege transition to system-mode
 	 */
irqtmr:
	pushl	%ebp			// push frame pointer
	movl	%esp, %ebp		// store stack pointer
	pushl	%eax			// back %eax up
	movl	%gs:8, %eax		// per-cpu current task
	cmpl	$0, %eax
	jnz		_save
	popl	%eax
	movl	%ebp, %esp
	popl	%ebp
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif
	sti
	iretl
_save:
	// adjust sp for pushing m_tcb.genregs
	leal	52(%eax), %esp
	pushal				// push general-purpose registers
	/* push segment registers */
	pushl	%gs			// kernel thread-local storage
	pushl	%fs			// user thread-local storage?
	pushl	%es			// buffer segment? something... :)
	pushl	%ds			// data segment
	/* store FPU context */
	/* %esp points to top of segregs */
	movl	(%eax), %ebx		// fetch flg
	leal	128(%eax), %ecx		// ECX = &task->fpstate
	andl	$M_FPMASK, %ebx		// (flg & M_FPMASK) ?
	cmpl	$M_NOFP, %ebx		// if zero, branch to _fpunone
	je	_fpunone		// no FPU-context saved
	cmpl	$M_FP387, %ebx		// check for i387
	je	_fnsave
	fxsave	(%ecx)			// SSE-context detected
	jmp	_fpusaved
_fnsave:
	fnsave	(%ecx)
_fpusaved:
	fwait
_fpunone:
	/* %esp points to m_task->tcb */
	movl	8(%ebp), %edx		// EDX = pushed %cs
	movl	(%ebp), %ebx		// EBX = stored frame pointer
	movl	%cr3, %ecx		// ECX = %cr3 (PDBR)
	cmpl	$UTEXTSEL, %edx		// check for user-mode interrupt
	jne	_getsysstk		// stack-info was not pushed if not
	movl	$UDATASEL, %esi		// ESI = user data-segment selector
	jmp	_havestk		// determined stack parameters
_getsysstk:
	/* construct stack information */
	movl	$DATASEL, %esi		// ESI = system stack/data selector
_havestk:
	leal	4(%ebp), %edi		// EDI = trap frame addr
	movl	%ecx, 52(%eax)		// task->pdbr = PDBR
	movl	%edx, 68(%eax)		// task->frame.cs = caller %cs
	movl	%esi, 80(%eax)		// task->frame.ss = SS
	movl	-4(%ebp), %ecx		// ECX = pushed %eax
	movl	12(%ebp), %edx		// EDX = pushed flags register
	movl	%ebx, 76(%eax)		// patch stack pointer in m_tcb.frame
	movl	4(%ebp), %esi		// ESI = pushed return address
	movl	%ecx, 48(%eax)		// patch %eax in m_tcb.genregs
	movl	%edx, 72(%eax)		// frame->eflags = pre-interrupt value
	movl	%esi, 64(%eax)		// frame.eip = return address
	movl	%edi, 84(%eax)		// m_task.trapesp = trap frame address
	/* schedule next thread */
	/* task structure address is still in %eax */
	movl	%ebx, 28(%eax)		// task->genregs.ebp = pre-trap EBP
	movl	%ebp, %esp		// restore stack pointer
	/* copy of task pointer is still in EAX */
//	pushl	%eax			// store task pointer
	call	schedswitchtask		// call scheduler function
	/* schedswitchtask does not return normally */
#if 0
	/* continue the same task */
//	popl	%ebx			// pop earlier task pointer
	leal	20(%eax), %esp
	popal
	movl	32(%esp), %esp		// ESP = m_task.trapesp
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif
	sti
	iretl
#endif

.align	16

irqkbd:
	pushl	$0
	pushl	$IRQKBD
	_trapenter
	movl	%esp, %ebp
	movl	$irqvec, %eax
	movl	4(%eax), %ebx
	jz		_irqkbddone
	call	*%ebx
_irqkbddone:
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif

	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

irq3:
	pushl	$0
	pushl	$IRQCOM2AND4
	_trapenter
	movl	%esp, %ebp
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif

	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

irq4:
	pushl	$0
	pushl	$IRQCOM1AND3
	_trapenter
	movl	%esp, %ebp

#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif

	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

irq5:
	pushl	$0
	pushl	$IRQLPT
	_trapenter
	movl	%esp, %ebp

	movl	$irqvec, %eax
	movl	20(%eax), %ebx
	jz		_irq5done
	call	*%ebx
_irq5done:
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif

	movl	%ebp, %esp
	_trapleave
	sti
	iretl

.align	16

irqmouse:
	pushl	$0
	pushl	$IRQMOUSE
	_trapenter
	movl	%esp, %ebp

	movl	$irqvec, %eax
	movl	48(%eax), %ebx
	jz		_irqmousedone
	call	*%ebx
_irqmousedone:
#if (APIC)
	_apiceoi
#else
	_piceoi1
	_piceoi2
#endif

	movl	%ebp, %esp
	_trapleave
	sti
	iretl

#if (APIC) || (SMP)

.align	16

irqerror:
	sti
	iretl

.align	16

irqspurious:
	_trapenter
	_apiceoi
	_trapleave
	sti
	iretl

#endif /* APIC || SMP */

.bss

.align	NBPG

	/* interrupt vector */
	/* 0x00..0x1f	- IRQs 0-15 */
	/* 19		- IRQERROR */
	/* 29		- IRQIPI */
	/* 31		- IRQSPURIOUS */
irqvec:
	.space	(8 * TRAPSMAX)

