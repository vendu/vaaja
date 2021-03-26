#ifndef __KERN_UNIT_IA32_TASK_H__
#define __KERN_UNIT_IA32_TASK_H__

#if !defined(__ASSEMBLER__)

#include <kern/conf.h>
#include <zero/cdefs.h>
#include <kern/cpu.h>
#include <kern/proc/task.h>

FASTCALL NORETURN void m_taskjmp(volatile struct m_task *task);

#endif /* !defined(__ASSEMBLER__) */

/* flg-bits for struct m_task */
//#define m_fputype(mt) ((mt)->fpflg & ~M_FPMASK)
#define M_NOFP   0
#define M_FP387  1
#define M_FPXMM  2
#define M_FPMASK (M_FP387 | M_FPXMM)
#define M_FPUSED 0x80000000
//#define M_FPMASK (M_FPUSED)

#if !defined(__ASSEMBLER__)

#define k_errnoloc() (&(k_getcurtask()->errnum))

static __inline__ struct cpu *
k_getcurcpu(void)
{
    struct cpu *cpu;

    __asm__ __volatile__ ("movl %%gs:0, %0\n"
                          : "=r" (cpu));

    return cpu;
}

static __inline__ long
k_getcurunit(void)
{
    long unit;

    __asm__ __volatile__ ("movl %%gs:0, %0\n"
                          : "=r" (unit));

    return unit;
}

static __inline__ struct task *
k_getcurtask(void)
{
    struct task *task;

    __asm__ __volatile__ ("movl %%gs:0, %0\n"
                          : "=r" (task));

    return task;
}

static __inline__ long
k_getcurpid(void)
{
    long pid;

    __asm__ __volatile__ ("movl %%gs:0, %0\n"
                          : "=r" (pid));

    return pid;
}

static __inline__ void
k_setcurcpu(volatile struct cpu *cpu)
{
    __asm__ __volatile__ ("movl %0, %%gs:0\n"
                          :
                          : "r" (cpu));

    return;
}

static __inline__ void
k_setcurunit(long unit)
{
    __asm__ __volatile__ ("movl %0, %%gs:4\n"
                          :
                          : "r" (unit));

    return;
}

static __inline__ void
k_setcurtask(struct task *task)
{
    __asm__ __volatile__ ("movl %0, %%gs:8\n"
                          :
                          : "r" (task));

    return;
}

static __inline__ void
k_setcurpid(long pid)
{
    __asm__ __volatile__ ("movl %0, %%gs:12\n"
                          :
                          : "r" (pid));

    return;
}

static __inline__ void
taskinittls(long unit, long id)
{
    volatile struct cpu *cpu = &k_cputab[unit];
    struct task         *task = k_tasktab[id];

    k_setcurcpu(cpu);
    k_setcurunit(unit);
    k_setcurtask(task);
    k_setcurpid(id);

    return;
}

#endif

#endif /* __KERN_UNIT_IA32_TASK_H__ */

