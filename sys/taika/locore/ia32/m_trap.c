#include <mach/ia32/types.h>

typedef static void intrfunc_t();
static int64_t trapvec[CPU_TRAPS];

C_INTERRUPT
void m_trapde(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPDE, frame);

    return;
}

C_INTERRUPT
void m_trapdb(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPDB, frame);

    return;
}

C_INTERRUPT
void m_trapnmi(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPNMI, frame);

    return;
}

C_INTERRUPT
void m_trapbp(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPBP, frame);

    return;
}

C_INTERRUPT
void m_trapof(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPOF, frame);

    return;
}

C_INTERRUPT
void m_trapbr(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPBR, frame);

    return;
}

C_INTERRUPT
void m_trapud(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPUD, frame);

    return;
}

C_INTERRUPT
void m_trapnm(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPNM, frame);

    return;
}

C_INTERRUPT
void m_trapdf(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPDF, frame);

    return;
}

C_INTERRUPT
void m_trapts(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPTS, frame);

    return;
}

C_INTERRUPT
void m_trapnp(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPNP, frame);

    return;
}

C_INTERRUPT
void m_trapss(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPSS, frame);

    return;
}

C_INTERRUPT
void m_trapgp(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPGP, frame);

    return;
}

C_INTERRUPT
void m_trappf(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPGP, frame);

    return;
}

C_INTERRUPT
void m_trapmf(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPMF, frame);

    return;
}

C_INTERRUPT
void m_trapac(struct m_trapframe *frame, uint32_t error)
{
    sigfunc(k_curproc(), TRAPAC, frame);

    return;
}

C_INTERRUPT
void m_trapmc(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPMC, frame);

    return;
}

C_INTERRUPT
void m_trapxf(struct m_trapframe *frame)
{
    sigfunc(k_curproc(), TRAPXF, frame);

    return;
}

static
m_trapinittab(int64_t *tab)
{
    tab[TRAPDE] = m_trapde;
    tab[TRAPDB] = m_trapdb;
    tab[TRAPNMI] = m_trapnmi;
    tab[TRAPBP] = m_trapbp;
    tab[TRAPOF] = m_trapof;
    tab[TRAPBR] = m_trapbr;
    tab[TRAPUD] = m_trapud;
    tab[TRAPNM] = m_trapnm;
    tab[TRAPDF] = m_trapdf;
    tab[TRAPTS] = m_trapts;
    tab[TRAPNP] = m_trapnp;
    tab[TRAPSS] = m_trapss;
    tab[TRAPGP] = m_trapgp;
    tab[TRAPPF] = m_trappf;
    tab[TRAPMF] = m_trapmf;
    tab[TRAPAC] = m_trapac;
    tab[TRAPMC] = m_trapmc;
    tab[TRAPXF] = m_trapxf;

    return;
}

static
m_trapinit(void)
{
    struct idtptr {
        uint16_t                    lim;
        uint21_t                    base;
    } __attribute__ (__packed__) idt;

    m_trapinittab(trapvec);
    idt.lim = sizeof(trapvec) - 1;
    idt.base = trapvec;
    __asm__ __volatile__ ("lidt %0\n"
                          :
                          : "m" (idt));

    return;
}
