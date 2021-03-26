#define __ASSEMBLER__ 1
#include <kern/unit/x86/boot.h>
#include <kern/syscallnum.h>
#include <errno.h>

/*
 * - pass system call number in EAX
 * - up to 3 arguments in EBX, ECX, and EDX
 * - set carry-flag on error return; EAX stores errno
 * - if carry not set, EAX is return value
 * - in case of EINTR, EBX is final return value
 */

/*
 * TODO
 * ----
 * - fix _ksyscall to deliver pending unmasked signals
 * - make _ksyscall check for another thread to run
 * - do system call profiling
 */

.globl	ksyscall, sysvec, errno

.text	32

/* TODO: fix ksyscall() call convention */

ksyscall:
	pushal
	movl	%esp, %ebp
	movl	$_sysfunc, %edi
	movl	(%edi, %eax, 4), %esi
	call	*%esi
	
_ksyscall0:
	movl	$sysvec, %edi
	cmpl	$0, %eax
	jle	_invlsys
	cmpl	$NSYSCALL, %eax
	jge	_invlsys
	movl	(%edi, %eax, 4), %esi
	call	*%esi
	/* finish system call */
	jnc	_success
	jmp	_failure

_ksyscall1:
	movl	$sysvec, %edi
	cmpl	$0, %eax
	jle	_invlsys
	cmpl	$NSYSCALL, %eax
	jge	_invlsys
	pushl	%ebx
	movl	(%edi, %eax, 4), %esi
	call	*%esi
	/* finish system call */
	jnc	_success
	jmp	_failure

_ksyscall2:
	movl	$sysvec, %edi
	cmpl	$0, %eax
	jle	_invlsys
	cmpl	$NSYSCALL, %eax
	jge	_invlsys
	pushl	%ecx
	pushl	%ebx
	movl	(%edi, %eax, 4), %esi
	call	*%esi
	/* finish system call */
	jnc	_success
	jmp	_failure

_ksyscall3:
	movl	$sysvec, %edi
	cmpl	$0, %eax
	jle	_invlsys
	cmpl	$NSYSCALL, %eax
	jge	_invlsys
	pushl	%edx
	pushl	%ecx
	pushl	%ebx
	movl	(%edi, %eax, 4), %esi
	call	*%esi
	/* finish system call */
	jnc	_success
	jmp	_failure

_invlsys:
	movl	$ENOSYS, errno
	movl	%ebp, %esp
	popal
	movl	$-1, %eax
	sti
	iret

_failure:
//	cmpl	$EINTR, %eax
//	je	_intr
	movl	%ebp, %esp
	movl	%eax, errno
	popal
	movl	$-1, %eax
	sti
	iret

#if 0
_intr:
	movl	%eax, errno
	/* pop actual return value */
	popl	%eax
	movl	%ebp, %esp
	movl	%eax, retval
	popal
	movl	retval, %eax
	sti
	iret
#endif

_success:	
	movl	%eax, retval
	movl	%ebp, %esp
	popal
	movl	retval, %eax
	sti
	iret

.data

errno:
	.long	0x00000000

retval:
	.long	0x00000000

.align NBPG

_sysfunc:
	.long	0x00000000
	.long	_ksyscall3	// sysctl
	.long	_ksyscall2	// exit
	.long	_ksyscall0	// abort
	.long	_ksyscall1	// fork
	.long	_ksyscall3	// exec
	.long	_ksyscall3	// throp
	.long	_ksyscall3	// pctl
	.long	_ksyscall3	// sigop
	.long	_ksyscall1	// brk
	.long	_ksyscall3	// map
	.long	_ksyscall2	// umap
	.long	_ksyscall3	// mhint
	.long	_ksyscall3	// mctl
	.long	_ksyscall3	// shmget
	.long	_ksyscall3	// shmat
	.long	_ksyscall1	// shmdt
	.long	_ksyscall3	// shmctl
	.long	_ksyscall3	// mnt
	.long	_ksyscall2	// umnt
	.long	_ksyscall3	// readdir
	.long	_ksyscall3	// open
	.long	_ksyscall2	// trunc
	.long	_ksyscall1	// close
	.long	_ksyscall3	// read
	.long	_ksyscall3	// readv
	.long	_ksyscall3	// write
	.long	_ksyscall3	// writev
	.long	_ksyscall3	// seek
	.long	_ksyscall3	// falloc
	.long	_ksyscall3	// stat
	.long	_ksyscall3	// fhint
	.long	_ksyscall3	// ioctl
	.long	_ksyscall3	// fctl
	.long	_ksyscall3	// poll
	.long	_ksyscall2	// select
	.long	_ksyscall0	// seminit
	.long	_ksyscall1	// semup
	.long	_ksyscall1	// semdown
	.long	_ksyscall3	// semop
	.long	_ksyscall3	// mqinit
	.long	_ksyscall3	// mqsend
	.long	_ksyscall3	// mqrecv
	.long	_ksyscall3	// mqop
	.long	_ksyscall3	// evreg
	.long	_ksyscall3	// evsend
	.long	_ksyscall3	// evrecv
	.long	_ksyscall3	// evop
sysvec:
	/* TODO: fill with sys_ function addresses */
	.space	4 * NSYSCALL

