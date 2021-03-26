#define  __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/bios.h>
#include <kern/unit/x86/boot.h>

/* GRUB support */
_MBMAGIC	= MBMAGIC
_MBFLAGS	= MBFLAGS
_MBCHKSUM	= MBCHKSUM

/* globals */
.globl	_start, start
#if !(LOCORE)
.globl	k_intrvec
#endif
.globl	k_pagedir
#if defined(__x86_64__) || defined(__amd64__)
.globl	k_pagetab4, k_pagetab3, k_pagetab2, k_pagetab1
#endif
.globl	k_tsstab, k_segtab, k_tsstab
.globl	k_iomap, k_sysstk, k_usrstk
.extern	kmain

.text	32

/* kernel entry */
/* MULTIBOOT header; must be in first 8 kilobytes of kernel image */
mboothdr:
.long	_MBMAGIC
.long	_MBFLAGS
.long	_MBCHKSUM
.long	0			// header_addr
.long	0			// load_addr
.long	0			// load_end_addr
.long	0			// bss_end_addr
.long	0			// entry_addr
.long	0			// video mode type (linear)
.long	GFXWIDTH	// video width
.long	GFXHEIGHT	// video height
.long	GFXDEPTH	// video depth

.align	4

	/* kernel startup */
_start:
start:
	cli										// disable interrupts
	movw	$BIOSWARMREBOOT, BIOSREBOOTMODE
	jmp	    _start2							// flush CPU pipeline
	nop

_start2:
	/* set kernel stack up */
	xorl	%ebp, %ebp						// zero frame pointer
	movl	$BOOTSTKTOP, %esp				// set bootstrap stack up
	pushl	%ebp							// push zero frame pointer
	movl	%esp, %ebp						// store frame pointer
	pushl	%ebx							// push boot-header address for kmain()
#if (LONGMODE)
  	call	_chkcpuid						// probe support for CPUID-instruction
	cmp		$0, %eax						// return value 0?
	jeq		_nolongmode						// yep, CPUID is not supported
#endif
_longmode:
	call	_chklongmode					// check if we want 64-bit long mode
	pushl	%eax							// push longmode argument for kmain()
	call	kmain
_nolongmode:
    xorl    %eax, %eax						// zero longmode argument for kmain()
	pushl	%eax							// push longmode argument
	call	kmain							// call kmain()

/*
 * check for presence of CPUID by trying to flip bit 21 in %eflags
 */
_chkcpuid:
	pushfl									// push EFLAGS
	popl	%eax							// EAX = EFLAGS
	movl	%eax, %ecx						// ECX = EFLAGS
	xorl	$(1 << 21), %eax				// EAX = EFLAGS ^ (1 << 21)
	pushl	%eax							// push EFLAGS ^ (1 << 21)
	popfl									// pop EFLAGS
	pushfl									// push EFLAGS with bit 21 flipped if CPUID
	popl	%eax							// EAX = EFLAGS
	pushl	%ecx							// push call-time EFLAGS
	popfl									// pop call-time EFLAGS
	xorl	%ecx, %eax						// check if bit 21 flipped?
	ret										// return non-zero if CPUID is supported

_chklongmode:
	movl	$(1 << 31), %eax				// call CPUID with EAX = 0x80000000
	cpuid
	cmp		$((1 << 31) | 1), %eax			// compare return value with 0x80000001
	jge		_longmodefin					// at least equal, long mode supported
	xorl	%eax, %eax						// long mode not supported, return zero
_longmodefin:								// long mode supported, return non-zero
	ret

.bss

.align	NBPG

/* IDT; interrupt descriptor table; shared between processors */
k_intrvec:
	.space	NBPG
/* page directory index page */
k_pagedir:
	.space	NBPG

#if defined(__x86_64__) || defined(__amd64__)

/* 64-bit memory management */

/* page-map level-4 table */
k_pagetab4:
	.space	NBPG
/* page directory pointer table */
k_pagetab3:
	.space	NBPG
/* page directory table */
k_pagetab2:
	.space	NBPG
/* page table */
k_pagetab1:
	.space	NBPG

#endif /* 64-bit */

/* per-CPU GDTs; kernel segment descriptor tables */
.align	NBPG

/* kernel TSS structures */
k_tsstab:
	.space	(CPUSMAX * TSSLEN)

	.align	NBPG

#if (SMP)

k_segtab:
#if (KERNSEGS < 16)
	.space	(16 * 8 * CPUSMAX)
#else
	.space	(8 * KERNSEGS * CPUSMAX)
#endif

#else /* !SMP */

k_segtab:
	.space	(8 * KERNSEGS)

#endif /* SMP */

/* kernel I/O protection bitmap */
k_iomap:
	.space	8192

/* kernel stacks */
k_sysstk:
	.space	(CPUSMAX * KERNSTKSIZE)
k_usrstk:
	.space	(CPUSMAX * KERNSTKSIZE)

