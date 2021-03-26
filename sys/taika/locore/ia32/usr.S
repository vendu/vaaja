#define __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/boot.h>

_KERNSTKSIZE = KERNSTKSIZE

.extern k_usrstk

.globl	m_jmpusr

.text	32

/* void m_jmpusr(long id, void *func);
 * - cpu ID in %eax
 * - function pointer in %edx */
m_jmpusr:
	pushl	%ebp
	movl	%ebp, %esp
	incl	%eax
	movl	$k_usrstk, %esi
//	pushl	%eax
	pushl	%edx
	movl	$UDATASEL, %ebx		// user data segment
	movl	$_KERNSTKSIZE, %edi
	movl	$UTEXTSEL, %ecx		// user text segment
	//	movl	$kernpagedir, %ecx	// page directory address
	mull	%edi				// stack address
//	movl	%esp, %esi			// stack pointer
	movw	%bx, %ds			// data segment
	movw	%bx, %es			// ES segment
	addl	$_KERNSTKSIZE, %eax
	addl	%esi, %edi			// &kernuserstk[id]
	popl	%edx
	movl	%edi, %ebp			// set frame pointer
//	popl	%eax
//	addl	$36, %esi			// adjust
	///	movl	%eax, %esp
	/* construct iret return frame (struct m_jmpframe) */
	pushl	%ebx				// user stack segment
	pushl	%edi				// user stack pointer
	pushfl						// push machine status word
	pushl	%ecx				// push user text/code segment selector
	pushl	%edx				// function to trigger
//	movl	%ecx, %cr3
//	movl	%esp, %ebp
	iretl

