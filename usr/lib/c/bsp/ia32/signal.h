#ifndef __BSP_IA32_SIGNAL_H__
#define __BSP_IA32_SIGNAL_H__

#include <features.h>

typedef int sig_atomic_t;

#if (USEBSD) && (!USEPOSIX)

/* sigcontext compatibility names */
#define sc_sp   tcb.frame.uesp
#define sc_fp   tcb.genregs.ebp
#define sc_pc   tcb.frame.eip
#define sc_ps   tcb.frame.eflags
#define sc_ds   tcb.segregs.ds
#define sc_es   tcb.segregs.es
#define sc_fs   tcb.segregs.fs
#define sc_gs   tcb.segregs.gs
#define sc_edi  tcb.genregs.edi
#define sc_esi  tcb.genregs.esi
#define sc_ebp  tcb.genregs.ebp
#define sc_esp  tcb.genregs.esp
#define sc_ebx  tcb.genregs.ebx
#define sc_edx  tcb.genregs.edx
#define sc_ecx  tcb.genregs.ecx
#define sc_eax  tcb.genregs.eax
#define sc_eip  tcb.frame.eip
#define sc_cs   tcb.frame.cs
#define sc_efl  tcb.frame.eflags
#define sc_uesp tcb.frame.uesp
#define sc_ss   tcb.frame.ss

#endif /* (USEBSD) && (!USEPOSIX) */

#endif /* __BSP_IA32_SIGNAL_H__ */

