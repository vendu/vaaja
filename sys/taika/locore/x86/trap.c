#include <kern/conf.h>
#include <stdint.h>
#include <signal.h>
#include <sys/io.h>
#include <mach/param.h>
#include <mach/types.h>
//#include <kern/syscall.h>
#include <kern/asm.h>
#include <kern/unit/x86/boot.h>
#include <kern/unit/x86/trap.h>
#if (APIC)
#include <kern/unit/x86/apic.h>
#endif

//extern void ksyscall(void);

extern void picinit(void);
extern void idtset(void);

extern void trapde(void);
extern void trapdb(void);
extern void trapnmi(void);
extern void trapbp(void);
extern void trapof(void);
extern void trapbr(void);
extern void trapud(void);
extern void trapnm(void);
extern void trapdf(void);
extern void trapts(void);
extern void trapnp(void);
extern void trapss(void);
extern void trapgp(void);
extern void trappf(void);
extern void trapmf(void);
extern void trapac(void);
extern void trapmc(void);
extern void trapxf(void);

extern void irqtmr0(void);
extern void irqtmr(void);
extern void irqkbd(void);
extern void irqmouse(void);

#if (SMP)
extern volatile long   mpmultiproc;
extern uint64_t        k_intrvec[CPUSMAX][TRAPSMAX];
#else
extern uint64_t        k_intrvec[TRAPSMAX];
#endif
extern struct m_farptr idtptr;

long                   k_trappriotab[TRAPSMAX];
long                   k_trapsigmap[TRAPSMAX]
= {
    SIGFPE,
    0,
    0,
    SIGTRAP,
    0,
    SIGBUS,
    SIGILL,
    SIGILL,
    0,
    0,
    0,
    SIGSEGV,
    SIGSTKFLT,
    SIGSEGV,
    0,
    0,
    SIGFPE,
    SIGBUS,
    SIGABRT,
    SIGFPE
};

#define trapsetprio(irq, prio)                                          \
    (k_trappriotab[(irq)] = (prio))

#if (PTRSIZE == 4)

void
trapsetidt(long ntrap, uint64_t *idt)
{
    idtptr.lim = ntrap * sizeof(uint64_t) - 1;
    idtptr.adr = (uint32_t)idt;
    idtset();

    return;
}

#endif

void
trapinitidt(long unit)
{
#if (SMP)
    uint64_t *idt = &k_intrvec[unit][0];
#else
    uint64_t *idt = &k_intrvec[0];
#endif

    trapsetintrgate(&idt[TRAPDE], trapde, TRAPSYS);
    trapsetintrgate(&idt[TRAPDB], trapdb, TRAPSYS);
    trapsetintrgate(&idt[TRAPNMI], trapnmi, TRAPSYS);
    trapsetintrgate(&idt[TRAPBP], trapbp, TRAPSYS);
    trapsetintrgate(&idt[TRAPOF], trapof, TRAPSYS);
    trapsetintrgate(&idt[TRAPBR], trapbr, TRAPSYS);
    trapsetintrgate(&idt[TRAPUD], trapud, TRAPSYS);
    trapsetintrgate(&idt[TRAPNM], trapnm, TRAPSYS);
    trapsetintrgate(&idt[TRAPDF], trapdf, TRAPSYS);
    trapsetintrgate(&idt[TRAPTS], trapts, TRAPSYS);
    trapsetintrgate(&idt[TRAPNP], trapnp, TRAPSYS);
    trapsetintrgate(&idt[TRAPSS], trapss, TRAPSYS);
    trapsetintrgate(&idt[TRAPGP], trapgp, TRAPSYS);
    trapsetintrgate(&idt[TRAPPF], trappf, TRAPSYS);
    trapsetintrgate(&idt[TRAPMF], trapmf, TRAPSYS);
    trapsetintrgate(&idt[TRAPAC], trapac, TRAPSYS);
    trapsetintrgate(&idt[TRAPMC], trapmc, TRAPSYS);
    trapsetintrgate(&idt[TRAPXF], trapxf, TRAPSYS);
    /* system call trap */
    trapsetintrgate(&idt[TRAPSYSCALL], ksyscall, TRAPUSER);
    /* IRQs */
#if (APIC) && 0
    trapsetintrgate(&idt[trapirqid(IRQTMR)], irqtmr0, TRAPUSER);
#else
    trapsetintrgate(&idt[trapirqid(IRQTMR)], irqtmr, TRAPUSER);
#endif
    trapsetintrgate(&idt[trapirqid(IRQKBD)], irqkbd, TRAPUSER);
    trapsetintrgate(&idt[trapirqid(IRQMOUSE)], irqmouse, TRAPUSER);
#if (APIC)
    trapsetintrgate(&idt[trapirqid(IRQERROR)], irqerror, TRAPUSER);
    trapsetintrgate(&idt[trapirqid(IRQSPURIOUS)], irqspurious, TRAPUSER);
#endif
    /* initialize interrupts */
    trapsetidt(TRAPSMAX, idt);

    return;
}

void
trapinitprio(void)
{
    trapsetprio(IRQKBD, 0);
    trapsetprio(IRQMOUSE, 1);
    trapsetprio(IRQIDE0, 2);
    trapsetprio(IRQIDE1, 3);
}

void
trapinit(long unit)
{
    trapinitidt(unit);
    trapinitprio();
    picinit();  // initialise interrupt controllers
    /* mask timer interrupt, enable other interrupts */
    outb(0x01, 0x21);
    outb(0x00, 0xa1);
    k_intron();
//    __asm__ __volatile__ ("sti\n");
//    pitinit();  // initialise interrupt timer

    return;
}

