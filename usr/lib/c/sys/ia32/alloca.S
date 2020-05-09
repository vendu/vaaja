#if defined(__i386__) && !defined(__GNUC__)

.globl alloca

.text 32

/*
 * stack at call time
 * ------------------
 * ...
 * size argument
 * return address <- stack pointer
 */
alloca:
	popl	%ebx			// return address
	popl	%eax			// size argument
	/* round up to multiple of 4 */
	addl	$0x03, %eax
	andl	$0xfffffffc, %eax
	/* reserve space + extra popped space */
	subl	%eax, %esp
	subl	$0x04, %esp
	movl	%esp, %eax
	pushl	%eax			// popped by caller
	pushl	%ebx			// return address
	ret

#endif

