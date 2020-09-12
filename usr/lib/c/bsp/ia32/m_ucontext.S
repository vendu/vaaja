.globl	m_getcontext, m_swapcontext

.text 32

.align 32

	/* ASMLINK void m_getcontext(ucontext_t *uc); */
m_getcontext:
	/* prologue */
	pushl	%ebp
	movl	%esp, %ebp
	/* load uc-argument into %eax */
	movl	8(%ebp), %eax
	/* load uc->gregs into %esp */
	movl	(%eax), %esp
	/* push general purpose registers */
	pushal
	/* adjust stack pointer for uc->iret */
	addl	$52, %esp
	/* load return address into %eax */
	movl	4(%ebp), %eax
	/* load caller frame address into %ebx */
	movl	(%ebp), %ebx
	/* push %ss into uc->iret */
	pushl	%ss
	/* push caller frame address for stack pointer */
	pushl	%ebx
	/* push %eflags */
	pushfl
	/* push %cs */
	pushl	%cs
	/* push return address */
	pushl	%eax
	/* epilogue */
	movl	%ebp, %esp
	popl	%ebp
	ret

	/* ASMLINK void m_setcontext(ucontext_t *uc); */
m_setcontext:	
	/* prologue */
	pushl	%ebp
	movl	%esp, %ebp
	/* load uc-argument into %eax */
	movl	8(%ebp), %eax
	/* load uc->gregs into %ebx */
	movl	(%eax), %ebx
	/* pop general purpose registers */
	popal
	/* switch to new context */
	iretl

