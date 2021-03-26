#include <kern/conf.h>

#if (SMP) || (APIC)

#include <stdint.h>
#include <kern/mem/vm.h>
#include <kern/unit/x86/kern.h>
#include <kern/unit/x86/ioapic.h>
#include <kern/unit/x86/trap.h>
#include <kern/unit/x86/mp.h>
#include <kern/unit/ia32/mp.h>

void
ioapicinit(long id)
{
    volatile struct cpu    *cpu = (struct cpu *)&k_cputab[id];
    volatile struct ioapic *ioapic = k_mp.ioapic;
    long                    ntrap;
//    long id;
    long                    l;

    if (!k_mp.multiproc) {

        return;
    }
    if (cpu == mpbootcpu) {
        vmmapseg((uint32_t)ioapic, (uint32_t)ioapic,
                 (uint32_t)((uint8_t *)ioapic + PAGESIZE),
                 PAGEPRES | PAGEWRITE);
    }
    ntrap = (ioapicread(ioapic, IOAPICVER) >> 16) & 0xff;
    for (l = 0 ; l < ntrap ; l++) {
        ioapicwrite(ioapic, IOAPICDISABLED | (IRQTMR + l), IOAPICTAB + 2 * l);
        ioapicwrite(ioapic, 0, IOAPICTAB + 2 * l + 1);
    }

    return;
}

void
ioapicsetirq(long irq, long id)
{
    volatile struct ioapic *ioapic = k_mp.ioapic;

    if (!k_mp.multiproc) {

        return;
    }
    ioapicwrite(ioapic, IRQTMR + irq, IOAPICTAB + 2 * irq);
    ioapicwrite(ioapic, id << 24, IOAPICTAB + 2 * irq + 1);

    return;
}

#endif /* SMP */

