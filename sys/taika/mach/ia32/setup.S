#define  __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/boot.h>

.globl	gdtinit, pginit, idtset, idtptr
.globl	realgdtptr
.globl	gdtptr, gdtptrtab
.extern	k_pagedir

.text	32

.align	4

	/*
	 * initialize segmentation.
	 * - load segment registers DS, ES, FS, and SS with the DATA selector
	 * - GS is per-CPU segment
	 * - set code segment register CS up with a far jump
	 */
gdtinit:
	cli
	lgdt	(%eax)
	movw	$DATASEL, %cx
	movw	$CPUSEL, %dx
	movw	%cx, %ds
	movw	%cx, %es
	movw	%cx, %fs
	movw	%cx, %ss
	movw	%dx, %gs
	ljmp	$TEXTSEL, $_gdtret
_gdtret:
	sti
	ret

	/*
	 * initialize paging.
	 * - load page directory physical address into CR3
	 * - set the PG-bit in CR0
	 * - finish with a near jump
	 */
pginit:
	movl	$k_pagedir, %eax
	movl	%eax, %cr3
	movl	%cr0, %eax
	orl		$CR0PG, %eax
	movl	%eax, %cr0
	jmp		_pgret
_pgret:
	ret

idtset:
	lidt	(idtptr)
	ret

.align	8

realgdtptr:
	.short	0x0000
	.long	0x00000000
gdtptr:
gdtptrtab:
	.space	8 * CPUSMAX
idtptr:
	.short	0x0000
	.long	0x00000000

