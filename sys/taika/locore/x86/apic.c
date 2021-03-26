#include <kern/conf.h>

#if (APIC) || (SMP)

#include <stdint.h>
#include <sys/io.h>
#include <zero/trix.h>
#include <kern/asm.h>
#include <kern/util.h>
#include <kern/cpu.h>
#include <kern/mem/vm.h>
#include <kern/unit/x86/bios.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/x86/pic.h>
#include <kern/unit/x86/pit.h>
#include <kern/unit/x86/apic.h>
#include <kern/unit/x86/link.h>

extern void      irqtmr(void);
extern void      irqtmrcnt(void);
extern uint64_t  k_intrvec[TRAPSMAX];
extern void     *irqvec[TRAPSMAX];

volatile struct cpu *mpbootcpu;
volatile uint32_t   *mpapic;
static uint32_t      apicrate;

/* TODO: fix this kludge */
void
kusleep(unsigned long nusec)
{
    nusec <<= 20;
    while (nusec--) {
        ;
    }
}

uint32_t *
apicprobe(void)
{
    uint64_t  val = APICBASE | APICENABLE;
    uint32_t *ptr;

    k_writemsr(APICMSR, val);
    val = k_readmsr(APICMSR);
    val &= 0xfffff000;
    ptr = (uint32_t *)(uint32_t)val;

    return ptr;
}

void
apicstarttmr(volatile uint32_t *base)
{
    /*
     * timer counts down at bus frequency from apicrate and issues
     * the interrupt trapirqid(Irqtmr)
     */
    apicsendirq(base, 0, APICBCAST | APICINIT | APICLEVEL, 200);
    while (base[APICINTRLO] & APICDELIVS) {
        ;
    }
    apicwrite(base, 0, APICTASKPRIO);
    apicwrite(base, apicrate, APICTMRINITCNT);
    //    apicwrite(base, 0x03, APICTMRDIVCONF);
    //    apicwrite(base, trapirqid(IRQTMR) | APICPERIODIC, APICTMR);

    return;
}

void
apicinittmr(volatile uint32_t *base)
{
    uint64_t cnt;
    uint32_t ntick;
    uint32_t rate;
    uint32_t freq;
    uint32_t tmrcnt;
    uint8_t  tmp8;

    /* initialise timer, mask interrupts, set divisor */
    apicwrite(base, IRQSPURIOUS | APICSWENABLE, APICSPURIOUS);
    apicwrite(base, 0x03, APICTMRDIVCONF);
    apicwrite(base, trapirqid(IRQTMR) | APICPERIODIC, APICTMR);
    apicwrite(base, 0xffffffff, APICTMRINITCNT);
    /* enable local APIC; set spurious interrupt vector */
    /* initialise PIT channel 2 in one-shot mode, 100 Hz frequency */
    tmp8 = inb(PITCTRL2) & 0xfd;
    tmp8 |= PITONESHOT;
    outb(tmp8, PITCTRL2);
    apicwrite(base, trapirqid(IRQTMR) | APICPERIODIC, APICTMR);
    pitsethz(100, PITCHAN2);
    while (!(inb(PITCTRL2) & 0x20)) {
        ;
    }
    tmp8 = inb(PITCTRL2) & 0xfe;
    outb(tmp8, PITCTRL2);
    outb(tmp8 | 1, PITCTRL2);
    ntick = 0xffffffff - apicread(base, APICTMRCURCNT);
    apicwrite(base, APICMASKED, APICTMR);
    apicwrite(base, APICMASKED, APICINTR0);
    apicwrite(base, APICMASKED, APICINTR1);
    /* disable performance counter overflow interrupts where available */
    if (((apicread(base, APICVER) >> 16) & 0xff) >= 4) {
        apicwrite(base, APICMASKED, APICPERFINTR);
    }
    /* map error interrupt to IRQERROR */
    apicwrite(base, IRQERROR, APICERROR);
    /* clear error status registers */
    apicwrite(base, 0, APICERRSTAT);
    apicwrite(base, 0, APICERRSTAT);
    /* number of ticks in 10 milliseconds */
    kprintf("tcks in 10 milliseconds: %lu\n", (unsigned long)ntick);
    ntick >> 2; // divide by 4 to get rate for 250 Hz
    rate = ntick / 250;
    kprintf("ticks per slice: %lu\n", (unsigned long)rate);
    apicrate = rate;
    /* acknowledge outstanding interrupts */
    apicwrite(base, 0, APICEOI);
#if 0
    apicwrite(base, 0, APICINTRHI);
    apicwrite(base, APICBCAST | APICINIT | APICLEVEL, APICINTRLO);
    while (base[APICINTRLO] & APICDELIVS) {
        ;
    }
#endif

    return;
}

void
apicinit(volatile uint32_t *base)
{
    /* identity-map APIC to kernel virtual address space */
    kprintf("local APIC @ 0x%p\n", base);
    /* identity-map MP APIC table */
    kprintf("APIC: map MP APIC (%d bytes) @ 0x%lx\n",
            PAGESIZE, (unsigned long)base);
    vmmapseg((uint32_t)base, (uint32_t)base,
             (uint32_t)((uint8_t *)base + PAGESIZE),
             PAGEPRES | PAGEWRITE | PAGENOCACHE);
    trapsetintrgate(&k_intrvec[IRQTMR], irqtmr, TRAPUSER);
    trapsetintrgate(&k_intrvec[IRQERROR], irqerror, TRAPUSER);
    trapsetintrgate(&k_intrvec[IRQSPURIOUS], irqspurious, TRAPUSER);
    apicinittmr(base);

    return;
}

void
apicstart(uint8_t unit, uint32_t *base)
{
    uint16_t *warmreset = (uint16_t *)BIOSWRV;

    outb(0x0f, RTCBASE);
    outb(0x0a, RTCBASE + 1);
    warmreset[0] = 0;
    warmreset[1] = (uint32_t)(base) >> 4;
    /* INIT IPI */
    apicsendirq(base, unit << 24, APICINIT | APICLEVEL | APICASSERT, 200);
    while (apicread(base, (APICINTRLO) & APICDELIVS)) {
        ;
    }
    apicsendirq(base, unit << 24, APICINIT | APICLEVEL, 0);
    while (apicread(base, APICINTRLO) & APICDELIVS) {
        ;
    }
    apicsendirq(base,
                unit << 24,
                APICASSERT | APICSTART | (uint32_t)base >> 12, 200);
    apicsendirq(base,
                unit << 24,
                APICASSERT | APICSTART | (uint32_t)base >> 12, 200);
    apicinit(base);

    return;
}

#endif /* APIC || SMP */

