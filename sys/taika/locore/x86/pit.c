#include <stddef.h>
#include <stdint.h>
#include <mach/asm.h>
#include <sys/io.h>
#include <zero/trix.h>
#include <kern/conf.h>
#include <kern/asm.h>
#include <kern/util.h>
#include <kern/printf.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/x86/pit.h>
#include <kern/unit/x86/pic.h>

extern uint64_t      k_intrvec[TRAPSMAX];
extern void         *irqvec[];
extern void          irqtmr0(void);
extern void          irqtmr(void);

void
pitinit(void)
{
    uint64_t *idt = k_intrvec;

    kprintf("initialising timer interrupt to %d Hz\n", HZ);
    trapsetintrgate(&idt[trapirqid(IRQTMR)], irqtmr, TRAPUSER);
    /* initialise timer */
    outb(PITCMD, PITCTRL);
    pitsethz(HZ, PITCHAN0);

    return;
}

#if 0
void
pitsleep(long msec)
{
    unsigned long n = 1000L * msec;

    while (n--) {
//        iodelay();
        ;
    }

    return;
}
#endif

/*
 * sleep for msec milliseconds, then call trigger func
 * only to be used before the APIC timers and scheduler are enabled
 */
void
pitsleep(unsigned long msec)
{
    unsigned long hz = max(1, divu1000(msec));
    unsigned long tmp = 1000L/msec;

    /* enable timer interrupt, disable other interrupts */
    irqvec[IRQTMR] = NULL;
    outb(~0x01, PICMASK1);
    outb(~0x00, PICMASK2);
    trapsetintrgate(&k_intrvec[trapirqid(IRQTMR)], irqtmr0, TRAPUSER);
    outb(PITDUALBYTE | PITONESHOT, PITCTRL);
    pitsethz(hz, PITCHAN0);
    k_waitint();
    /* enable all interrupts */
    outb(0x00, PICMASK1);
    outb(0x00, PICMASK2);

    return;
}

