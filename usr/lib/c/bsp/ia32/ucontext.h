#ifndef __BSP_IA32_UCONTEXT_H__
#define __BSP_IA32_UCONTEXT_H__

#define mc_onstack ctx.onstk
#define mc_gs      ctx.segregs.gs
#define mc_fs      ctx.segregs.fs
#define mc_es      ctx.segregs.es
#define mc_ds      ctx.segregs.ds
#define mc_edi     ctx.genregs.edi
#define mc_esi     ctx.genregs.esi
#define mc_ebp     ctx.genregs.ebp
#define mc_esp     ctx.genregs.esp
#define mc_ebx     ctx.genregs.ebx
#define mc_edx     ctx.genregs.edx
#define mc_ecx     ctx.genregs.ecx
#define mc_eax     ctx.genregs.eax
#define mc_eip     ctx.frame.eip
#define mc_cs      ctx.frame.cs
#define mc_efl     ctx.frame.eflags
#define mc_esp     ctx.frame.uesp
#define mc_ss      ctx.frame.ss

#endif /* __BSP_IA32_UCONTEXT_H__ */

