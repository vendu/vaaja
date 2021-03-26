#define __ASSEMBLER__ 1
#include <kern/conf.h>
#include <kern/unit/x86/pic.h>
#if (APIC) || (SMP)
#include <kern/unit/x86/apic.h>
#endif

/* INTERRUPT MANAGEMENT */

.macro	_trapenter
	pushal
.endm
.macro	_trapleave
	popal
	addl	$8, %esp	/* trap number and [possibly fake] error code */
.endm
.macro	_trapentererr
	pushal
	movl	32(%esp), %ecx	/* move error code to %ecx */
.endm

#if (APIC) || (SMP)
.macro	_apiceoi
	/* send APIC EOI (end of interrupt) */
	movl	$APICEOI, APICEOIADR
.endm
#endif /* APIC */

.macro	_piceoi1
	/* send EOI to PIC1 */
	movb	$PICEOI, %al
	outb	%al, $PIC1
.endm
.macro	_piceoi2
	/* send EOI to PIC2 */
	outb	%al, $PIC2
.endm

