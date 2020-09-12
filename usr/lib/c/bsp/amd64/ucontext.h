#ifndef __BSP_X86_64_UCONTEXT_H__
#define __BSP_X86_64_UCONTEXT_H__

#define mc_onstack ctx.onstk
#define mc_r8      ctx.genregs.r8
#define mc_r9      ctx.genregs.r9
#define mc_r10     ctx.genregs.r10
#define mc_r11     ctx.genregs.r11
#define mc_r12     ctx.genregs.r12
#define mc_r13     ctx.genregs.r13
#define mc_r14     ctx.genregs.r14
#define mc_r15     ctx.genregs.r15
#define mc_rdi     ctx.genregs.rdi
#define mc_rsi     ctx.genregs.rsi
#define mc_rbp     ctx.genregs.rbp
#define mc_rbx     ctx.genregs.rbx
#define mc_rdx     ctx.genregs.rdx
#define mc_rcx     ctx.genregs.rcx
#define mc_rax     ctx.genregs.rax
#define mc_rip     ctx.frame.rip
#define mc_rfl     ctx.frame.rflags
#define mc_cs      ctx.frame.cs
#define mc_fs      ctx.frame.fs
#define mc_gs      ctx.frame.gs
#define mc_rsp     ctx.frame.ursp
#define mc_ss      ctx.frame.ss

#endif /* __BSP_X86_64_UCONTEXT_H__ */

