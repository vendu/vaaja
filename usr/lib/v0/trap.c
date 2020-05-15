#include <v0/types.h>
#include <v0/trap.h>
#include <zen/signal.h>

typedef                 void (*v0trapfunc)(struct v0trapframe *);

static v0trapdesc       _trapvec[V0_MAX_TRAPS];
static int8_t           _sigmap[V0_MAX_TRAPS];

static void
v0tmrirq(struct v0trapframe *frame)
{

    return;
}

static void
v0kbdirq(struct v0trapframe *frame)
{

    return;
}

static void
v0ptrirq(struct v0trapframe *frame)
{

    return;
}

static void
v0hidirq(struct v0trapframe *frame)
{

    return;
}

static void
v0audirq(struct v0trapframe *frame)
{

    return;
}

static void
v0vidirq(struct v0trapframe *frame)
{

    return;
}

static void
v0netirq(struct v0trapframe *frame)
{

    return;
}

static void
v0dskirq(struct v0trapframe *frame)
{

    return;
}

static void
v0optirq(struct v0trapframe *frame)
{

    return;
}

static void
v0usbirq(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0a(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0b(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0c(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0d(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0e(struct v0trapframe *frame)
{

    return;
}

static void
v0irq0x0f(struct v0trapframe *frame)
{

    return;
}

#if 0
static void
v0initsigmap(void)
{
    _sigmap[V0_DZ_TRAP] = SIGFPE;
    _sigmap[V0_OP_TRAP] = SIGILL;
    _sigmap[V0_TF_TRAP] = SIGBUS;
    _sigmap[V0_TS_TRAP] = SIGABRT;
    _sigmap[V0_NP_TRAP] = SIBGUS;
    _sigmap[V0_SS_TRAP] = SIGSTKFLT;
    _sigmap[V0_PE_TRAP] = SIGSTKFLT;
    _sigmap[V0_FP_TRAP] = SIGFPE;
    _sigmap[V0_BR_TRAP] = SIGBUS;
    _sigmap[V0_AC_TRAP] = SIGBUS;
    _sigmap[V0_MF_TRAP] = SIGABRT;
    _sigmap[V0_MC_TRAP] = SIGABRT;
    _sigmap[V0_IO_TRAP] = SIGIOT;
    _sigmap[V0_BP_TRAP] = SIGSTOP;

    return;
}

#define v0trapdesc(func, flg)   ((uintptr_t)(f) | (flg));
static void
v0inittrapvec(void)
{
    _trapvec[V0_IRQ_0x00_TRAP] = v0trapdesc(v0tmrirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x01_TRAP] = v0trapdesc(v0audirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x02_TRAP] = v0trapdesc(v0vidirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x03_TRAP] = v0trapdesc(v0kbdirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x04_TRAP] = v0trapdesc(v0pntirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x05_TRAP] = v0trapdesc(v0hidirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x06_TRAP] = v0trapdesc(v0netirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x07_TRAP] = v0trapdesc(v0dskirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x08_TRAP] = v0trapdesc(v0optirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x09_TRAP] = v0trapdesc(v0usbirq, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0a_TRAP] = v0trapdesc(v0irq0x0a, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0b_TRAP] = v0trapdesc(v0irq0x0b, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0c_TRAP] = v0trapdesc(v0irq0x0c, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0d_TRAP] = v0trapdesc(v0irq0x0d, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0e_TRAP] = v0trapdesc(v0irq0x0e, V0_SYS_TRAP_BIT);
    _trapvec[V0_IRQ_0x0f_TRAP] = v0trapdesc(v0irq0x0f, V0_SYS_TRAP_BIT);

    return;
}
#endif /* 0 */

static void
v0inittraps(void)
{
#if 0
    v0initsigmap();
    v0inittrapvec();
#endif

    return;
}

