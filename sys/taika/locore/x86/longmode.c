#if defined(__x86_64__) || defined(__amd64__)

#include <kern/conf.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <zero/param.h>
#include <zero/types.h>
#include <zero/trix.h>
#include <kern/util.h>
#include <kern/obj.h>
#include <kern/sched.h>
#include <kern/cpu.h>
#include <kern/proc/proc.h>
#include <kern/mem/page.h>
#include <kern/io/drv/chr/cons.h>
//#include <kern/task.h>
#include <kern/io/drv/pc/dma.h>
#include <kern/io/drv/pc/vga.h>
#if (ACPI)
#include <kern/io/drv/pc/acpi.h>
#endif
#include <kern/unit/x86/boot.h>
#include <kern/unit/x86/pic.h>
#include <kern/unit/x86/pit.h>
#include <kern/unit/x86/kern.h>
#include <kern/unit/x86/link.h>
#include <kern/unit/x86-64/vm.h>
#if (SMP) || (APIC)
//#include <kern/unit/x86-64/mp.h>
#endif
#if (VBE)
#include <kern/unit/x86/trap.h>
#endif
//#include <kern/unit/x86/asm.h>

#if (VBE)
extern void trapinitprot(void);
#endif
extern void cpuinit(volatile struct m_cpu *cpu);
extern long bufinit(void);

#if (HPET)
extern void hpetinit(void);
#endif
#if (SMP)
extern void mpstart(void);
#endif
#if (VBE)
extern void idtinit(uint64_t *idt);
extern void vbeinitscr(void);
extern void vbeprintinfo(void);
#endif
#if (SMBIOS)
extern void smbiosinit(void);
#endif
#if (PS2DRV)
extern void ps2init(void);
#endif
#if (PLASMA)
extern void plasmaloop(void);
#endif
#if (PCI)
extern void pciinit(void);
#endif
#if (ATA)
extern void atainit(void);
#endif
#if (ACPI)
extern void acpiinit(void);
#endif
#if (SB16)
extern void sb16init(void);
#endif
#if (APIC)
uint32_t    apicinitcpu(long id);
extern void apicstarttmr(uint32_t tmrcnt);
#endif
extern void schedloop(void);

extern uint8_t                   kerniomap[8192] ALIGNED(PAGESIZE);
extern struct proc               proctab[NTASK];
extern struct m_cpu              cputab[NCPU];
#if (VBE)
extern uint64_t                  kernidt[NINTR];
extern long                      vbefontw;
extern long                      vbefonth;
#endif
#if (SMP)
#if (ACPI)
extern volatile struct acpidesc *acpidesc;
#endif
#endif

void
kinitlong(unsigned long pmemsz)
{
#if (NEWTMR)
    uint32_t tmrcnt = 0;
#endif

/* initialise interrupt management */
#if (VBE)
    trapinitprot();
#endif
    /* initialise virtual memory */
    vminitlong((uint64_t *)kernpagemapl4tab);
#if 0
    /* FIXME: map possible device memory */
    vmmapseg((uint32_t *)&_pagetab, DEVMEMBASE, DEVMEMBASE, 0xffffffffU,
             PAGEPRES | PAGEWRITE | PAGENOCACHE);
#endif
//    schedinit();
    /* zero kernel BSS segment */
    kbzero(&_bssvirt, (uint32_t)&_ebssvirt - (uint32_t)&_bssvirt);
    /* set kernel I/O permission bitmap to all 1-bits */
    kmemset(&kerniomap, 0xff, sizeof(kerniomap));
    /* INITIALIZE CONSOLES AND SCREEN */
#if (VBE)
    vbeinitscr();
#endif
#if (VBE) && (NEWFONT)
    consinit(768 / vbefontw, 1024 / vbefonth);
#elif (VBE)
    consinit(768 >> 3, 1024 >> 3);
#endif
    /* TODO: use memory map from GRUB? */
//    vminitphys((uintptr_t)&_epagetab, pmemsz);
    vminitphys((uintptr_t)&_epagetab, pmemsz);
    meminit(pmemsz);
    tssinit(0);
#if (VBE) && (NEWFONT)
//    consinit(768 / vbefontw, 1024 / vbefonth);
#elif (VBE)
    consinit(768 >> 3, 1024 >> 3);
#endif
#if (SMBIOS)
    smbiosinit();
#endif
#if (PS2DRV)
    ps2init();
#endif
#if (VBE) && (PLASMA)
    plasmaloop();
#endif
#if (VBE)
    vbeprintinfo();
#endif
    logoprint();
//    vminitphys((uintptr_t)&_ebss, pmemsz - (unsigned long)&_ebss);
    /* HID devices */
#if (PCI)
    /* initialise PCI bus driver */
    pciinit();
#endif
#if (ATA)
    /* initialise ATA driver */
    atainit();
#endif
#if (SB16)
    /* initialise Soundblaster 16 driver */
    sb16init();
#endif
#if (ACPI)
    /* initialise ACPI subsystem */
    acpiinit();
#endif
    /* initialise block I/O buffer cache */
    if (!bufinit()) {
        kprintf("failed to allocate buffer cache\n");

        while (1) {
            ;
        }
    }
    /* allocate unused device regions (in 3.5G..4G) */
//    pageaddzone(DEVMEMBASE, &vmshmq, 0xffffffffU - DEVMEMBASE + 1);
#if (SMP) || (APIC)
//#if (SMP)
    /* multiprocessor initialisation */
//    mpinit();
//#endif
    if (mpncpu == 1) {
        kprintf("found %ld processor\n", mpncpu);
    } else {
        kprintf("found %ld processors\n", mpncpu);
    }
#if (HPET)
    /* initialise high precision event timers */
    hpetinit();
#endif
#if (NEWTMR)
    tmrcnt = apicinitcpu(0);
#else
    apicinitcpu(0);
#endif
#if (IOAPIC)
    ioapicinit(0);
#endif
#endif /* SMP || APIC */
#if (SMP)
    if (mpmultiproc) {
        mpstart();
    }
#endif
    /* CPU interface */
    taskinit();
//    tssinit(0);
//    machinit();
    /* execution environment */
    procinit(PROCKERN);
//    k_curtask = &k_curproc->task;
//    sysinit();
    kprintf("DMA buffers (%ul x %ul kilobytes) @ 0x%p\n",
            DMANCHAN, DMACHANBUFSIZE >> 10, DMABUFBASE);
    kprintf("VM page tables @ 0x%p\n", (unsigned long)&_pagetab);
//    kprintf("%ld kilobytes physical memory\n", pmemsz >> 10);
    kprintf("%ld kilobytes kernel memory\n", (uint32_t)&_ebss >> 10);
    kprintf("%ld kilobytes allocated physical memory (%ld wired, %ld total)\n",
            (vmpagestat.nwired + vmpagestat.nmapped + vmpagestat.nbuf) << (PAGESIZELOG2 - 10),
            vmpagestat.nwired << (PAGESIZELOG2 - 10),
            vmpagestat.nphys << (PAGESIZELOG2 - 10));
    k_curcpu = &cputab[0];
    cpuinit(k_curcpu);
    schedinit();
#if (APIC)
    apicstarttmr(tmrcnt);
#else
    pitinit();
#endif
    schedloop();

    /* NOTREACHED */
}

#endif /* __x86_64__ || __amd64__ */

