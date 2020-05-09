#if defined(__arm__) && !defined(__GNUC__)

.globl alloca

.text 32

/*
 * registers at call time
 * ----------------------
 * r0	- size argument
 * sp	- stack pointer
 * lr	- return address
 */
alloca:
	add	r0, r0, $0x3
	and	r0, r0, $0xfffffffc
	sub	sp, sp, r0				// subtract size from stack pointer
	mov	r0, sp					// set return value
	bx	lr						// return

#endif

