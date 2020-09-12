#ifndef __BSP_X86_64_SIGNAL_H__
#define __BSP_X86_64_SIGNAL_H__

typedef long sig_atomic_t;

#if defined(_BSD_SOURCE) && !defined(_POSIX_SOURCE)

/* sigcontext compatibility names */
#define sc_sp   ctx.frame.ursp
#define sc_fp   ctx.genregs.rbp
#define sc_pc   ctx.frame.rip
#define sc_ps   ctx.frame.rflags
#define sc_cs   ctx.frame.cs
#define sc_fs   ctx.frame.fs
#define sc_gs   ctx.frame.gs
#define sc_r8   ctx.genregs.r8
#define sc_r9   ctx.genregs.r9
#define sc_r10  ctx.genregs.r10
#define sc_r11  ctx.genregs.r11
#define sc_r12  ctx.genregs.r12
#define sc_r13  ctx.genregs.r13
#define sc_r14  ctx.genregs.r14
#define sc_r15  ctx.genregs.r15
#define sc_rdi  ctx.genregs.rdi
#define sc_rsi  ctx.genregs.rsi
#define sc_rbp  ctx.genregs.rbp
#define sc_rsp  ctx.genregs.rsp
#define sc_rbx  ctx.genregs.rbx
#define sc_rdx  ctx.genregs.rdx
#define sc_rcx  ctx.genregs.rcx
#define sc_rax  ctx.genregs.rax
#define sc_rip  ctx.frame.rip
#define sc_cs   ctx.frame.cs
#define sc_rfl  ctx.frame.rflags
#define sc_ursp ctx.frame.ursp
#define sc_ss   ctx.frame.ss

#endif /* (USEBSD) && (!USEPOSIX) */

#endif /* __BSP_X86_64_SIGNAL_H__ */

