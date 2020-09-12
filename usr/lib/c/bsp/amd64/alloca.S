#if defined(__x86_64__) || defined(__amd64__) && !defined(__GNUC__)

.globl alloca

.text 64

/*
 * registers at call time
 * ----------------------
 * rdi	- size argument
 *
 * stack at call time
 * ------------------
 * return address <- stack pointer
 */
alloca:
	subq	$8, %rdi	// adjust for popped return address
	/* <FIXME> */
	addq	$7, %rdi
	andq	$0xfffffffffffffff8, %rdi
	/* </FIXME> */
	movq	%rsp, %rax	// copy stack pointer
	subq	%rdi, %rax	// reserve space; return value is in RAX
	ret			// return

#endif

