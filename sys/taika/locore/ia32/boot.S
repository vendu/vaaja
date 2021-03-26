_MBMAGIC = 0x1baDb002
_MBFLAGS = ((1 << 0) | (1 << 1))
_MBCHKSUM = (-(0x1bADb002 + ((1 << 0) | (1 << 1))))


.globl _start, start

.globl k_intrvec

.globl k_pagedir



.globl k_tsstab, k_segtab, k_tsstab
.globl k_iomap, k_sysstk, k_usrstk
.extern kmain

.text 32



mboothdr:
.long _MBMAGIC
.long _MBFLAGS
.long _MBCHKSUM
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 1024
.long 768
.long 24

.align 4


_start:
start:
 cli
 movw $0x1234, 0x472
 jmp _start2
 nop

_start2:

 xorl %ebp, %ebp
 movl $0x00080000, %esp
 pushl %ebp
 movl %esp, %ebp
 pushl %ebx





_longmode:
 call _chklongmode
 pushl %eax
 call kmain
_nolongmode:
    xorl %eax, %eax
 pushl %eax
 call kmain




_chkcpuid:
 pushfl
 popl %eax
 movl %eax, %ecx
 xorl $(1 << 21), %eax
 pushl %eax
 popfl
 pushfl
 popl %eax
 pushl %ecx
 popfl
 xorl %ecx, %eax
 ret

_chklongmode:
 movl $(1 << 31), %eax
 cpuid
 cmp $((1 << 31) | 1), %eax
 jge _longmodefin
 xorl %eax, %eax
_longmodefin:
 ret

.bss

.align 4096


k_intrvec:
 .space 4096

k_pagedir:
 .space 4096
# 130 "../x86/boot.S"
.align 4096


k_tsstab:
 .space (8 * 128)

 .align 4096



k_segtab:

 .space (16 * 8 * 8)
# 155 "../x86/boot.S"
k_iomap:
 .space 8192


k_sysstk:
 .space (8 * PAGESIZE)
k_usrstk:
 .space (8 * PAGESIZE)

