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
	movq	%rsp, %rax			// copy stack pointer
	subq	$0x08, %rdi			// adjust for popped return address
	addq	$0x07, %rdi
	andq	$0xfffffffffffffff8, %rdi
	subq	%rdi, %rax			// reserve space; return value is in RAX
	ret

#endif

