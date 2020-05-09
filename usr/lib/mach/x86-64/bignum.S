/* REFERENCE: on http://locklessinc.com/articles/256bit_arithmetic/ */

.globl	addu128, mulu128

.text	64

addu128:
	movq	%rdx, %rax
	movq	%rcx, %rdx
	addq	%rdi, %rax
	adcq	%rsi, %rdx
	retq

mulu128:
	imulq	%rdi, %rcx
	movq	%rdx, %rax
	imul	%rdx, %rsi
	addq	%rsi, %rcx
	mulq	%rdi
	addq	%rcx, %rdx
	retq

