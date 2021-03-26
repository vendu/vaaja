#ifndef __UNIT_IA32_VM_H__
#define __UNIT_IA32_VM_H__

#include <kern/conf.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <mach/asm.h>
#include <kern/unit/x86/link.h>

#define VMFLATPHYSTAB 1

extern long kernlongmode;

void vminitphys(uintptr_t base, m_ureg_t nbphys);
void vminitvirt(void *virt, m_ureg_t size, m_ureg_t flg);
void vmfreephys(void *virt, m_ureg_t size);
void vmmapseg(uintptr_t virt, uintptr_t phys, m_ureg_t lim,
              m_ureg_t flg);

#define KERNVIRTBASE      0xc0000000U // 3 gigabytes
#define KERNDEVBASE       (KERNVIRTBASE + 512 * 1024 * 1024)
#define KERNLOADBASE      0x01400000U // 20 megs, beyond flat page-table
//#define vmvirtadr(adr)    ((uint32_t)adr - KERNLOADBASE)
#define vmvirtadr(adr)    ((uint32_t)adr)
#define vmlinkadr(adr)    ((uint32_t)(adr) - KERNVIRTBASE)
#define vmphysadr(adr)    ((uintptr_t)(((uint32_t *)&_pagetab)[vmpagenum(adr)]) & VMPAGEMASK)
#define vmpagedirnum(adr) ((uint32_t)(adr) >> PDSHIFT)
#define vmpagenum(adr)    ((uint32_t)(adr) >> PTSHIFT)
#define vmpageofs(adr)    ((uint32_t)(adr) & (PAGESIZE - 1))

#define vmpageid(adr)     ((uint32_t)(adr) >> PAGESIZELOG2)
#define vmbufid(adr)      ((uint32_t)(adr) >> PAGESIZELOG2)
#define vmisbufadr(adr)   (!((uint32_t)(adr) & (BUFSIZE - 1)))

/* internal macros */

static __inline__ void
vmflushtlb(void *adr)
{
    __asm__ __volatile__ ("invlpg (%0)\n" : : "r" (adr) : "memory");
}

/* physical memory limit; leave high areas for devices */
//#define DEVMEMBASE      0xe0000000      // 3.5 G

/* virtual memory parameters */
//#define NPAGEMAX        (PDESMAX * PTESMAX)   // # of virtual pages
#define PDESMAX         1024            // per directory
#define PTESMAX         1024            // per table
#define PAGETABSIZE     (PDESMAX * PTESMAX * sizeof(uint32_t))
#define PDSHIFT         22
#define PTSHIFT         12
#define VMPDMASK        0xffc00000      // top 10 bits
#define VMPTMASK        0x003ff000      // bits 12..21
#define VMPAGEMASK      0xfffff000U     // page frame; 22 bits

/* page structure setup */

#define vmtrylkpte(tab, ndx) m_trylkbit(&tab[ndx], PAGELOCKBITPOS)
#define vmlkpte(tab, ndx)    m_lkbit(&tab[ndx], PAGELOCKBITPOS)
#define vmunlkpte(tab, ndx)  m_clrbit(&tab[ndx], PAGELOCKBITPOS)

/*
 * page flags
 */
/* standard IA-32 flags */
#define PAGEPRES        0x00000001U	// present
#define PAGEWRITE       0x00000002U	// writeable
#define PAGEUSER        0x00000004U	// user-accessible
#define PAGEWRITETHRU   0x00000008U
#define PAGENOCACHE     0x00000010U     // disable caching
#define PAGEREF         0x00000020U	// has been referenced
#define PAGEDIRTY       0x00000040U	// has been written to
#define PAGESUPERPTE    0x00000080U	// 4M/2M page
#define PAGEGLOBAL      0x00000100U	// global
#define PAGESYS1        0x00000200U	// reserved for system
#define PAGESYS2        0x00000400U	// reserved for system
#define PAGESYS3        0x00000800U	// reserved for system
#define PAGELOCKBITPOS  11
#define PAGESUPERPMD    0x00001000U
/* custom flags */
#define PAGESWAPPED     PAGESYS1        // swapped out
#define PAGEWIRED       PAGESYS2        // wired
#define PAGELOCKED      PAGESYS3        // page-table entry locked
//#define PAGEBUF         PAGESYS2        // buffer cache
#define PAGESYSFLAGS    (PAGESWAPPED | PAGEWIRED | PAGELOCKED)

/* page fault management */

/* page fault exception */
#define NSWAPDEV        16
//#define pfltdev(adr)  (((adr) & PAGEFLTDEVMASK) >> 3)
#define pagefltadr(adr) ((adr) & PAGEFLTPAGEMASK)
#define PAGEFLTPRES     0x00000001U	// page is present
#define PAGEFLTWRITE    0x00000002U	// write fault
#define PAGEFLTUSER     0x00000004U	// user fault
#define PAGEFLTRESBIT   0x00000008U     // 1 in reserved bit
#define PAGEFLTINST     0x00000010U     // caused by instruction fetch
#define PAGEFLTFLGMASK  0x0000001fU
#define PAGEFLTADRMASK  0xffffffe0U
#define PAGEFLTPAGEMASK 0xfffff000U

struct vmpagemap {
    uintptr_t *dir; // page directory address
    uintptr_t *tab; // flat page-table of PDESMAX * PTESMAX entries
};

struct vmpagestat {
    unsigned long  nphys;
    unsigned long  nvirt;
    unsigned long  nmap;
    unsigned long  nwire;
    unsigned long  nbuf;
    void          *phys;
    void          *physend;
    void          *buf;
    void          *bufend;
};

#endif /* __UNIT_IA32_VM_H__ */

