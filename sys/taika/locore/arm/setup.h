#ifndef __UNIT_ARM_SETUP_H__
#define __UNIT_ARM_SETUP_H__

#include <zero/param.h>
#include <unit/arm/trap.h>

/*
 * stack organization
 * - interrupt stack is the highest page
 * - SVC stack is a page right below interrupt stack
 * - user stack starts at the page below SVC stack
 */
#define TRAPINTSTK   0x00000000
#define TRAPSVCSTK   0xfffff000
#define TRAPUSRSTK   (TRAPSVCSTK - 4096)

#define TRAPBXTBIT   0x00000001   // 16-bit thumb mode

#define TRAPPSRTBIT  0x00000020   // 16-bit thumb mode
#define TRAPPSRFBIT  0x00000040   // mask FIQs
#define TRAPPSRIBIT  0x00000080   // mask IRQs
#define TRAPPSRABIT  0x00000100
#define TRAPPSREBIT  0x00000200
#define TRAPPSRJBIT  0x01000000   // 8-bit jazelle mode
#define TRAPPSRQBIT  0x08000000   // saturation; sticky (needs to be cleared)
#define TRAPPSRVBIT  0x10000000   // signed overflow
#define TRAPPSRCBIT  0x20000000   // unsigned carry
#define TRAPPSRZBIT  0x40000000   // zero flag
#define TRAPPSRNBIT  0x80000000   // negative

#define TRAPPSRFLAGS 0xff000000
#define TRAPPSRSTAT  0x00ff0000
#define TRAPPSREXT   0x0000ff00
#define TRAPPSRCTL   0x000000ff

#define irqmask()   mrs r1, cpsr; orr r1, r1, #TRAPPSRIBIT; msr cpsr_c, r1;
#define irqunmask() mrs r1, cpsr; bic r1, r1, #TRAPPSRIBIT; msr cpsr_c, r1;
#define fiqmask()   mrs r1, cpsr; orr r1, r1, #TRAPPSRFBIT; msr cpsr_c, r1;
#define fiqunmask() mrs r1, cpsr; bic r1, r1, #TRAPPSRFBIT; msr cpsr_c, r1;

#endif /* __UNIT_ARM_SETUP_H__ */

