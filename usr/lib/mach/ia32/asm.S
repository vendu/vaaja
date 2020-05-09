.globl	asmgetpc

.text	32

asmgetpc:
	movl	(%esp), %eax
	ret

