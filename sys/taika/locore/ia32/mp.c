#if !defined(__x86_64__) && !defined(__amd64__)

#include <kern/conf.h>
//#include <kern/unit/x86/mp.h>

#if defined(SMP) || defined(APIC)

#include <stddef.h>
#include <stdint.h>
#include <sys/io.h>
#include <zero/trix.h>
#include <zero/cdefs.h>
#include <mach/asm.h>
#include <mach/param.h>
#include <mach/types.h>
#include <kern/asm.h>
#include <kern/util.h>
#include <kern/malloc.h>
#include <kern/cpu.h>
#include <kern/mem/vm.h>
#include <kern/proc/task.h>
#include <kern/proc/kern.h>
#include <kern/unit/x86/kern.h>
#include <kern/unit/x86/apic.h>
#include <kern/unit/ia32/mp.h>
#include <kern/unit/ia32/task.h>

/* used to scan for MP table */
#define EBDAADR   0x040e
#define TOPMEMADR 0x0413
#define TOPMEMDEF 0xa0000
/* MP table signature */
#define MPSIG 0x5f504d5f        // "_MP_"

extern void gdtinit(void);
extern void idtset(void);

extern uintptr_t *kernpagedir[PDESMAX];

struct k_mp k_mp;

static long
mpchksum(uint8_t *ptr, unsigned long len)
{
    uint8_t sum = 0;

    while (len--) {
        sum += *ptr++;
    }

    return sum;
}

static struct mp *
mpprobe(uintptr_t adr, unsigned long len)
{
    struct mp *mp = NULL;
    uint32_t  *ptr = (uint32_t *)adr;
    uint32_t  *lim = (uint32_t *)(adr + len);

    while (ptr < lim) {
        if (*ptr == MPSIG && !mpchksum((uint8_t *)ptr, sizeof(struct mp))) {
            mp = (struct mp *)ptr;

            return mp;
        }
        ptr++;
    }

    return mp;
}

/*
 * MP structure is in one of the following 3 locations
 * - the first KB of the EBDA
 * - the last KB of system base memory
 * - in the BIOS ROM between 0xf0000 and 0xfffff
 */
PURE struct mp *
mpsearch(void)
{
    uint32_t   adr = 0;
    struct mp *mp = NULL;

    adr = (uint32_t)(((uint16_t *)EBDAADR)[0] << 4);
    if (adr) {
        mp = mpprobe(adr, 1024);
    }
    if (mp) {

        return mp;
    }
    adr = (uint32_t)((((uint16_t *)TOPMEMADR)[0] << 10) - 1024);
    mp = mpprobe(adr - 1024, 1024);
    if (mp) {

        return mp;
    }
    if (adr != TOPMEMDEF - 1024) {
        adr = TOPMEMDEF - 1024;
        mp = mpprobe(adr, 1024);
        if (mp) {

            return mp;
        }
    }
    adr = 0xf0000;
    mp = mpprobe(adr, 0x10000);

    return mp;
}

static struct mpconf *
mpconf(struct mp **mptab)
{
    struct mp     *mp = mpsearch();
    struct mpconf *conf;

    if ((mp) && (mp->conftab)) {
        conf = mp->conftab;
        if (!kmemcmp(conf, "PCMP", 4)) {
            if ((conf->ver == 1 || conf->ver == 4)
                && !mpchksum((uint8_t *)conf, conf->len)) {
                *mptab = mp;

                return conf;
            } else {

                return NULL;
            }
        } else {

            return NULL;
        }
    }

    return NULL;
}

void
mpinitcpu(long unit, volatile uint32_t *apic)
{
    volatile struct cpu *cpu = &k_cputab[unit];

    cpuinit(cpu);
    cpu->unit = unit;
    cpu->apic = apic;
    /* TODO: initialise HPET; enable [rerouted] interrupts */
#if (HPET)
    hpetinit();
#endif
    apicinit(unit);
#if (IOAPIC)
    ioapicinit(unit);
#endif
    tssinit(unit);

    return;
}

void
mpinit(void)
{
    struct mp         *mp;
    struct mpconf     *conf;
    struct mpcpu      *cpu;
    volatile uint32_t *apic;
    struct mpioapic   *ioapic;
    long               unit;
    uint8_t           *u8ptr;
    uint8_t           *lim;

    conf = mpconf(&mp);
    if (!conf) {

        return;
    }
    apic = conf->apicadr;
    for (u8ptr = (uint8_t *)(conf + 1), lim = (uint8_t *)conf + conf->len ;
         u8ptr < lim ; ) {
        switch (*u8ptr) {
            case MPCPU:
                cpu = (struct mpcpu *)u8ptr;
                unit = cpu->id;
                if (cpu->flags & MPCPUBOOT) {
                    mpbootcpu = &k_cputab[unit];
                }
                mpinitcpu(unit, apic);
                k_cputab[unit].unit = unit;
                k_mp.ncpu++;
                u8ptr += sizeof(struct mpcpu);

                continue;
            case MPIOAPIC:
                ioapic = (struct mpioapic *)u8ptr;
                k_mp.ioapicid = ioapic->apicnum;
                k_mp.ioapic = (struct ioapic *)ioapic->adr;
                u8ptr += sizeof(struct mpioapic);

                continue;
            case MPBUS:
            case MPIOINTR:
            case MPLINTR:
                u8ptr += 8;

                continue;
            default:
                continue;

                break;
        }
    }
    if (k_mp.ncpu > 1) {
        k_mp.multiproc = 1;
    } else {

        return;
    }
    if (mp->intmode) {
        outb(0x70, 0x22);               // select IMCR
        outb(inb(0x23) | 0x01, 0x23);   // mask external timer interrupts
    }

    return;
}

ASMLINK NORETURN
void
mpmain(struct cpu *cpu)
{
    long unit = cpu->unit;

    seginit(unit);
    idtset();
    m_atomwrite((m_atomic_t *)&cpu->flg, CPUSTARTED);
#if 0
    unit = mpinitcpu();
#endif
    schedloop();

    /* NOTREACHED */
    for ( ; ; ) { ; }
}

#if 0
void
mpstart(void)
{
    volatile struct cpu *cpu;
    volatile struct cpu *lim;
    uint32_t            *mpentrystk = (uint32_t *)MPENTRYSTK;

    lim = &k_cputab[0] + k_mp.ncpu;
#if 0
    if (first) {
        kmemcpy((void *)MPENTRY,
                mpentry, (uint8_t *)&mpend - (uint8_t *)&mpentry);
        first = 0;
    }
#endif
    for (cpu = &k_cputab[0] ; cpu < lim ; cpu++) {
        if (cpu == k_mp.bootcpu) {
            /* started already */

            continue;
        }
        kprintf("starting CPU %ld @ 0x%lx\n", cpu->unit, MPENTRY);
        cpuinit(cpu);
        apicinit(cpu->unit);
        ioapicinit(cpu->unit);
        *--mpentrystk = (uint32_t)cpu;
        *--mpentrystk = MPENTRYSTK - cpu->unit * MPSTKSIZE;
        *--mpentrystk = (uint32_t)&kernpagedir;
        apicstart(cpu->unit, MPENTRY);
        while (!cpu->statflg) {
            ;
        }
    }

    return;
}
#endif

#endif /* SMP || APIC */

#endif /* !__x86_64__ && !__amd64__ */
