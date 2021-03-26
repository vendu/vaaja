#ifndef __KERN_UNIT_IA32_M_TRAP_H__
#define __KERN_UNIT_IA32_M_TRAP_H__

#include <zero/types.h>

CC_INTERRUPT void irqtmr(struct m_trapframe *trapframe);

#endif /*  __KERN_UNIT_IA32_M_TRAP_H__ */

