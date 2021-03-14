#ifndef __BSP_X86_64_SIGNAL_H__
#define __BSP_X86_64_SIGNAL_H__

typedef long sig_atomic_t;

#if defined(_BSD_SOURCE) && !defined(_POSIX_SOURCE)

/* sigcontext compatibility names */
#define sc_sp   tcb.frame.ursp
#define sc_fp   tcb.genregs.rbp
#define sc_pc   tcb.frame.rip
#define sc_ps   tcb.frame.rflags
#define sc_cs   tcb.frame.cs
#define sc_fs   tcb.frame.fs
#define sc_gs   tcb.frame.gs
#define sc_r8   tcb.genregs.r8
#define sc_r9   tcb.genregs.r9
#define sc_r10  tcb.genregs.r10
#define sc_r11  tcb.genregs.r11
#define sc_r12  tcb.genregs.r12
#define sc_r13  tcb.genregs.r13
#define sc_r14  tcb.genregs.r14
#define sc_r15  tcb.genregs.r15
#define sc_rdi  tcb.genregs.rdi
#define sc_rsi  tcb.genregs.rsi
#define sc_rbp  tcb.genregs.rbp
#define sc_rsp  tcb.genregs.rsp
#define sc_rbx  tcb.genregs.rbx
#define sc_rdx  tcb.genregs.rdx
#define sc_rcx  tcb.genregs.rcx
#define sc_rax  tcb.genregs.rax
#define sc_rip  tcb.frame.rip
#define sc_cs   tcb.frame.cs
#define sc_rfl  tcb.frame.rflags
#define sc_ursp tcb.frame.ursp
#define sc_ss   tcb.frame.ss

#endif /* (USEBSD) && (!USEPOSIX) */

#endif /* __BSP_X86_64_SIGNAL_H__ */

