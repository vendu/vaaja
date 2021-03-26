#ifndef __KERN_CONF_H__
#define __KERN_CONF_H__

#define ZERO_KERNEL (defined(__KERNEL__))

#include <mach/param.h>

#define PLASMA        1
#define PLASMAFOREVER 1
#define USYSINFO      0
#define MEMTKTLK      1
#define VMTKTLK       1
#define VMPAGETKTLK   1
#define BUFTKTLK      1

//#define REENTRANTGDTINIT 1
#define LONGMODE      0
#define MEMBUFNOLK    0 // FIXME: implement lock-free data (stack?)

#define RAMSIZE       (1U * 1024 * 1024 * 1024)
#define MEMPARANOIA   0
#define BUFDYNALLOC   1
#if (PTRBITS == 32)
#define MEMBUFBLKS    1024
#else
#define MEMBUFBLKS    32768
#endif
#define PERTHRSTACKS  0

#define VMFLATPHYSTAB 1
#define USERMODE      0

#define NEWFONT       1
#if !defined(BUFMEGS)
#define BUFMEGS       128
#endif

#define INET4         1

#define kgethz()      HZ
#define HZ            250       // scheduler timer frequency
#define SMP           1         // enable multiprocessor support; FIXME:  broken
#define SMT           1         // enable hardware multithreading ('hyper')
#define FASTHZ        1000      // fast timer frequency (interactive tasks etc.)
#define ZEROSCHED     1         // default scheduler (based on FreeBSD ULE)
#define ZEROINTSCHED  1         // scheduler with interactivity-scoring
#define TASKSMAX      1024      // maximum number of running tasks on system
#define TASKCHILDREN  16        // maximum number of children per task
#define PROCDESCS     4096      // maximum number of descriptors per process
#define TASKSTKSIZE   1048576   // task user mode stack size
#define KERNSTKSIZE   PAGESIZE  // task kernel mode stack size
#if (SMP)
#define CPUSMAX       8
#else
#define CPUSMAX       1
#endif
#if (SMT)
#define CORESMAX      8
#else
#define CORESMAX      1
#endif

/* NOTE: it's not recommended to edit anything below unless you develop Zero */

#define GFXWIDTH      1024      // horizontal screen resolution
#define GFXHEIGHT     768       // vertical screen resolution
#define GFXDEPTH      24        // bits per pixel

/* maximum number of physical pages */
#if (PTRBITS == 32)
#define PAGESMAX      (KERNVIRTBASE / PAGESIZE)
//#define PAGESPHYS     (RAMSIZE / PAGESIZE)
#define PAGESDEV      (PAGESMAX / 8)
#endif

#define LOCORE        0
#define CONSMAX       8

#define BOCHS         1
#define DEVEL         0         // debugging

#define APIC          1         // local [per-CPU] APIC support
#define HPET          1         // high precision event timer support
/* planned and developed drivers */
#define VBE           1         // VBE2 graphics driver
#define VBEMTRR       1         // map graphics framebuffer write-combining
#define PIC           0
#define IOAPIC        0         // enable I/O APIC support
#define SMBIOS        0         // SMBIOS (system management) support
#define PCI           1         // PCI bus driver
#define ATA           0         // ATA/ATAPI/SATA/SATAPI
#define ACPI          0         // ACPI
#define ACPICA        0         // ACPICA
#define PS2DRV        0         // enable PS/2 mouse and keyboard drivers
#define KBDUS         0         // PS/2 keyboard with US keymap
#define PS2KBDSET2    0
#define VGAGFX        0         // VGA graphics driver
#define SB16          0         // Soundblaster 16 audio driver
#define AC97          0         // AC97 audio drivers
#define ENS1370       0         // Ensoniq 1370 audio driver

#endif /* __KERN_CONF_H__ */

