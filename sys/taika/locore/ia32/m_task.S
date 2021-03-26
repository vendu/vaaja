#define __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/boot.h>
#include <kern/unit/x86/bits/cpu.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/ia32/macro.S>
#include <kern/unit/ia32/task.h>

	/* TODO: privilege transition(?), per-cpu segment (%gs), tss/ltr */

.globl	m_taskjmp

.text	32

.align	32

	/* FASTCALL NORETURN void m_taskjmp(struct m_task *task); */

m_taskjmp:
	movl	%gs:8, %eax
	/* fetch flgsave-word */
	movl	(%eax), %ebx
	andl	$M_FPMASK, %ebx
	leal	128(%eax), %edx
	cmpl	$M_NOFP, %ebx
	je	_fpunone
	cmpl	$M_FP387, %ebx
	je	_frstor
	fxrstor	(%edx)
	jmp	_fpudone
_frstor:
	frstor	(%edx)
_fpudone:
	fwait
_fpunone:
	/* fetch page directory page register */
	movl	52(%eax), %ebx
	/* restore page directory page register */
	movl	%ebx, %cr3
	/* - load stack pointer for segregs */
	leal	4(%eax), %esp
	popl	%ds
	popl	%es
	popl	%fs
	popl	%gs
	/* pop general purpose registers */
	popal
	movl	64(%esp), %esp
#if (APIC)
	_apiceoi
#else
	_piceoi1
#endif
	iretl

