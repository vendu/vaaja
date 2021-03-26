#define __ASSEMBLER__ 1
#include <kern/unit/ia32/boot.h>

.globl	_start, start
.globl	kernidt, kernpmaplvl4
.extern	kinit

.text	32

_start:
start:
	cli
	jmp	_start2

mboothdr:
.long	_MBMAGIC
.long	_MBFLAGS
.long	_MBCHKSUM

_start2:
	movl	$KERNSTKTOP, %esp
	movl	%esp, %ebp
	//	call	kinit
	call	_chkcpuid
	cmp	$0, %eax
	je	_protmode
	/* we have long mode */
	call	pginitlong

.align NBPG

kernidt:
	.space	NBPG
kernpmaplvl4:
	.space	NBPG

