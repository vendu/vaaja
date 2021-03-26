#include <kern/conf.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <mach/types.h>
#include <zero/trix.h>
#include <kern/util.h>
#include <kern/obj.h>
#include <kern/sched.h>
#include <kern/cpu.h>
#include <kern/proc/kern.h>
#include <kern/proc/proc.h>
#include <kern/mem/mem.h>
#include <kern/mem/page.h>
#include <kern/unit/x86/kern.h>
#include <kern/mem/vm.h>
#if 0
#if 0
#include <kern/mem/page.h>
#endif
#include <kern/io/drv/chr/cons.h>
//#include <kern/task.h>
#include <kern/io/drv/pc/vga.h>
#if (ACPI)
#include <kern/io/drv/pc/acpi.h>
#endif
#include <kern/unit/x86/boot.h>
#include <kern/unit/x86/pic.h>
#include <kern/unit/x86/pit.h>
#include <kern/unit/x86/link.h>
#if (SMP) || (APIC)
#include <kern/unit/ia32/mp.h>
#if (APIC)
#include <kern/unit/x86/apic.h>
#endif
#endif
#if (IOAPIC)
#include <kern/unit/x86/ioapic.h>
#endif
#if (VBE)
#include <kern/unit/x86/trap.h>
#endif
//#include <kern/unit/x86/asm.h>
#endif
#include <kern/io/drv/pc/dma.h>

struct vmpage                    k_vmdevtab[PAGESDEV];
extern uint8_t                   k_iomap[8192] ALIGNED(PAGESIZE);
extern uint8_t                   k_sysstk[CPUSMAX * KERNSTKSIZE];
extern uint8_t                   k_usrstk[CPUSMAX * KERNSTKSIZE];
extern struct proc               k_proctab[TASKSMAX];
#if (VBE)
extern uint64_t                  k_intrvec[TRAPSMAX];
extern long                      vbefontw;
extern long                      vbefonth;
#endif
#if (SMP)
#if (ACPI)
extern volatile struct acpidesc *acpidesc;
#endif
#endif

NORETURN void
kinitprot(unsigned long pmemsz)
{
    uint32_t lim = min(pmemsz, KERNVIRTBASE);
    uint32_t sp = (uint32_t)k_sysstk + KERNSTKSIZE;

    __asm__ __volatile__ ("movl %0, %%esp\n"
                          "pushl %%ebp\n"
                          "movl %%esp, %%ebp\n"
                          :
                          : "rm" (sp));
    /* initialise virtual memory */
    //    vminit((uint32_t *)&_pagetab);
    vminit();
    //    trapinit(0);
    //    schedinit();
    /* zero kernel BSS segment */
    kbzero((void *)&_bss,
           (uintptr_t)&_ebss - (uintptr_t)&_bss);
    /* set kernel I/O permission bitmap to all 1-bits */
    kmemset(k_iomap, 0xff, sizeof(k_iomap));
    /* INITIALIZE CONSOLES AND SCREEN */
    /* TODO: use memory map from GRUB? */
    meminit((m_ureg_t)&_ebss, RAMSIZE, KERNVIRTBASE);
    /* allocate unused device regions (in 3.5G..4G) */
    pageinitphyszone(KERNDEVBASE, k_vmdevtab, PAGESDEV);
    cpuinit(0);
    procinit(0, TASKKERN, SCHEDSYSTEM);
    taskinitenv();
    tssinit(0);
#if (VBE)
    vbeinitscr();
#if (PLASMA)
    plasmainit();
#endif
#endif
#if (VBE) && (NEWFONT)
    consinit(768 / vbefontw, 1024 / vbefonth);
#elif (VBE)
    consinit(768 >> 3, 1024 >> 3);
#endif
#if (PS2DRV)
    ps2init();
#endif
#if (SMP)
    /* multiprocessor initialisation */
    mpinit();
    if (k_mp.ncpu == 1) {
        kprintf("found %ld processor\n", k_mp.ncpu);
    } else {
        kprintf("found %ld processors\n", k_mp.ncpu);
    }
#if 0
    if (k_mp.multiproc) {
        mpstart();
    }
#endif
#elif (APIC)
    trapinitidt(0);
    apicinit(0);
#endif /* SMP */
#if (IOAPIC)
    ioapicinit(0);
#endif
//    procinit(TASKKERN, SCHEDNOCLASS);
    kprintf("%lu free physical pages @ 0x%p..0x%p\n",
            k_physmem.pagestat.nphys, k_physmem.pagestat.phys, k_physmem.pagestat.physend);
#if (SMBIOS)
    smbiosinit();
#endif
#if (VBE) && 0
    vbeprintinfo();
#endif
    logoprint();
//    vminitphys((uintptr_t)&_ebss, pmemsz - (unsigned long)&_ebss);
    /* HID devices */
#if (ATA)
    /* initialise ATA driver */
    atainit();
#endif
#if (PCI)
    /* initialise PCI bus driver */
    pciinit();
#endif
#if (AC97) && (PCI)
    /* initialise AC97 audio driver */
    ac97init();
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
    if (!ioinitbuf()) {
        kprintf("failed to allocate buffer cache\n");

        while (1) {
            ;
        }
    }
#if 0
    kprintf("%lu kilobytes of buffer cache @ %p..%p\n",
            k_physmem.pagestat.nbuf << (PAGESIZELOG2 - 10),
            k_physmem.pagestat.buf, k_physmem.pagestat.bufend);
#endif
    kprintf("DMA buffers (%ul x %ul kilobytes) @ 0x%x\n",
            DMACHANS, DMACHANBUFSIZE >> 10, DMABUFBASE);
    kprintf("VM page tables @ 0x%p\n", &_pagetab);
//    kprintf("%ld kilobytes physical memory\n", pmemsz >> 10);
    kprintf("%d kilobytes kernel memory\n", (uint32_t)&_ebss >> 10);
    kprintf("%ld kilobytes allocated physical memory (%ld wired, %ld total)\n",
            ((k_physmem.pagestat.nwire + k_physmem.pagestat.nmap + k_physmem.pagestat.nbuf)
             << (PAGESIZELOG2 - 10)),
            k_physmem.pagestat.nwire << (PAGESIZELOG2 - 10),
            k_physmem.pagestat.nphys << (PAGESIZELOG2 - 10));
    sysinitconf();
#if (PLASMA) && 0
    plasmainit();
#endif
#if (HPET)
    /* initialise high precision event timers */
    hpetinit();
#endif
    schedinit();
#if (APIC)
    apicstarttmr();
#else
    pitinit();
#endif
//    usrinit(&_usysinfo);
    do {
#if (PLASMAFOREVER)
        plasmaloop(-1);
#elif (USERMODE)
        m_jmpusr(0, schedloop);
#else
        schedloop();
#endif
    } while (1);

    /* NOTREACHED */
}

