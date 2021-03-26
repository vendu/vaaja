#ifndef __KERN_UNIT_X86_64_TASK_H__
#define __KERN_UNIT_X86_64_TASK_H__

#if !defined(__ASSEMBLER__)

#include <zero/cdefs.h>
#include <zero/param.h>
#include <zero/types.h>

FASTCALL NORETURN void m_taskjmp(struct m_task *task);

#endif /* !defined(__ASSEMBLER__) */

/* flg-bits for struct m_task */
#define m_fputype(mt) ((mt)->fpflg & ~M_FPMASK)
#define M_NOFP   0
#define M_FP387  1
#define M_FPXMM  2
#define M_FPUSED 0x80000000
#define M_FPMASK (M_FPUSED)

#endif /* __KERN_UNIT_X86_64_TASK_H__ */

