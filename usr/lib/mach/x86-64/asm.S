.globl	asmgetpc

.text	64

asmgetpc:
	movq	(%rsp), %rax
	ret

