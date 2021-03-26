/*
 * IA-32 page tables are accessed as a flat 4-megabyte set of page table
 * entries. The table is mapped to the address PAGETAB declared in vm.h.
 */

#define PAGEDEV 0

#include <kern/conf.h>
#include <stddef.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>
#include <mt/mtx.h>
#include <kern/util.h>
#include <kern/mem/page.h>
#include <kern/mem/vm.h>
#include <kern/io/drv/pc/dma.h>
#include <kern/unit/x86/link.h>
#include <kern/unit/ia32/seg.h>
#if (SMP)
#include <kern/unit/ia32/mp.h>
#endif

#define HICORE (1024 * 1024)

void pginit(void);

extern uint8_t   k_sysstk[CPUSMAX * KERNSTKSIZE];
extern uint8_t   k_usrstk[CPUSMAX * KERNSTKSIZE];
extern uintptr_t k_pagedir[PDESMAX];
extern uintptr_t k_usrpagedir[PDESMAX];
#if (VMFLATPHYSTAB)
struct vmpage    k_vmphystab[PAGESMAX] ALIGNED(PAGESIZE);
#endif
struct vmpage    k_vmdevtab[PAGESDEV];

//static struct vmpage  k_vmpagetab[PAGESMAX] ALIGNED(PAGESIZE);
struct k_physmem  k_physmem;

/*
 * 32-bit page directory is flat 4-megabyte table of page-tables.
 * for virtual address virt,
 *
 *     vmphysadr = pagetab[vmpagenum(virt)]; // physical address
 */
void
vmmapseg(uintptr_t virt, uintptr_t phys, m_ureg_t lim,
         m_ureg_t flg)
{
    uintptr_t *pte;
    m_ureg_t   n;

    n = rounduppow2(lim - virt, PAGESIZE) >> PAGESIZELOG2;
    pte = (uintptr_t *)&_pagetab + vmpagenum(virt);
    k_physmem.pagestat.nmap += n;
    while (n--) {
        *pte = phys | flg;
        phys += PAGESIZE;
        pte++;
    }

    return;
}

/*
 * initialise virtual memory
 * - no locking; call before going multiprocessor
 * - zero page tables
 * - initialize page directory
 * - map page directory index page
 * - map our segments
 * - initialise paging
 */
void
vminit(void)
{
    uintptr_t *pde;
    uintptr_t *pte;
    uintptr_t  adr;
    long       n;

    /* PHYSICAL MEMORY */

    /* initialize page directory index page */
    pde = k_pagedir;
    adr = (uintptr_t)&_pagetab;
    n = PDESMAX;
    while (n--) {
        *pde = adr | PAGEPRES | PAGEWRITE;
        adr += PTESMAX * sizeof(uintptr_t);
        pde++;
    }

    /* map page directory index page */
    pde = (uintptr_t *)&_pagetab + vmpagenum(k_pagedir);
    adr = (uintptr_t)&k_pagedir;
    *pde = adr | PAGEUSER | PAGEPRES | PAGEWRITE;

    /* zero page tables */
    kbzero((void *)&_pagetab, PAGETABSIZE);

    /* zero stacks */
#if 0
    kbzero(k_sysstk, CPUSMAX * KERNSTKSIZE);
    kbzero(k_usrstk, CPUSMAX * KERNSTKSIZE);
#endif
    /* map kernel-mode stacks */
    vmmapseg((uintptr_t)k_sysstk, (uintptr_t)k_sysstk,
             (uintptr_t)k_sysstk + CPUSMAX * KERNSTKSIZE,
             PAGEPRES | PAGEWRITE);
    /* map user-mode stacks */
    vmmapseg((uintptr_t)k_usrstk, (uintptr_t)k_usrstk,
             (uintptr_t)k_usrstk + CPUSMAX * KERNSTKSIZE,
             PAGEUSER | PAGEPRES | PAGEWRITE);
#if defined(__x86_64__) || defined(__amd64__)
    /* zero page structures */
    kbzero(kernpagetab4, 4 * PAGESIZE);
#endif

    /* identity-map 0..1M */
    vmmapseg(0, 0,
             HICORE,
             PAGEPRES | PAGEWRITE | PAGENOCACHE | PAGEWIRED);
#if (SMP)
    vmmapseg((uintptr_t)MPENTRY, (uintptr_t)MPENTRY,
             (uintptr_t)&mpend - (uintptr_t)&mpentry + MPENTRY,
             PAGEPRES);
#endif

    /* identity-map kernel low-half boot segment */
    vmmapseg(HICORE, HICORE,
             (uintptr_t)&_dmabuf,
             PAGEPRES | PAGEWRITE);

#if (USYSINFO)
    /* identity-map USYSINFO */
    vmmapseg((uintptr_t)&_usysinfo, (uintptr_t)&_usysinfo,
             (uintptr_t)&_eusysinfo,
             PAGEPRES | PAGEWRITE);
#endif

    /* identity-map kernel DMA buffers */
    vmmapseg((uintptr_t)&_dmabuf, DMABUFBASE,
             (uintptr_t)&_dmabuf + DMABUFSIZE,
             PAGEPRES | PAGEWRITE | PAGENOCACHE | PAGEWIRED);

    /* identity-map page tables */
    vmmapseg((uintptr_t)&_pagetab, (uintptr_t)&_pagetab,
             (uintptr_t)&_pagetab + PAGETABSIZE,
             PAGEPRES | PAGEWRITE | PAGEWIRED);

    /* identity map free RAM */
    vmmapseg((uintptr_t)&_epagetab,
             (uintptr_t)&_epagetab,
             KERNVIRTBASE - (uintptr_t)&_epagetab,
             PAGEWRITE);

    //    kbzero(&_epagetab, lim - (uintptr_t)&_epagetab);

    /* VIRTUAL MEMORY */

    /* map kernel text/read-only segments */
    vmmapseg(vmvirtadr((uintptr_t)&_text), vmlinkadr((uintptr_t)&_text),
             (uintptr_t)&_etext,
             PAGEPRES);

    /* map kernel DATA and BSS segments */
    vmmapseg(vmvirtadr((uintptr_t)&_data), vmlinkadr((uintptr_t)&_data),
             (uintptr_t)&_ebss,
             PAGEPRES | PAGEWRITE);

    /* identity-map 3.5G..4G */
// devmap(&_pagetab, DEVMEMBASE, 512 * 1024 * 1024);

    /* initialize paging */
    pginit();

    return;
}

void
vminitphys(uintptr_t base, m_ureg_t nbphys)
{
    m_ureg_t nb = min(nbphys, KERNVIRTBASE);

    /* initialise physical memory manager */
    pageinitphys(base, nb);

    return;
}

void
vminitvirt(void *virt, m_ureg_t size, m_ureg_t flg)
{
    void      *adr;
    uintptr_t *pte;
    long       n;

    n = rounduppow2(size, PAGESIZE) >> PAGESIZELOG2;
    pte = (uintptr_t *)&_pagetab + vmpagenum(virt);
    while (n--) {
        *pte = flg;
        pte++;
    }

    return;
}

void
vmfreephys(void *virt, m_ureg_t size)
{
//    struct vmbuf *buf;
    uintptr_t  adr;
    uintptr_t *pte;
    long       n;
//    long          nref;
//    struct vmpage  *pg;

    n = rounduppow2(size, PAGESIZE) >> PAGESIZELOG2;
    pte = (uintptr_t *)&_pagetab + vmpagenum(virt);
    while (n--) {
        adr = (uintptr_t)*pte;
        adr &= VMPAGEMASK;
        if (!adr) {

            continue;
        }

        if (*pte & PAGESWAPPED) {
//            swapfree(adr);
        } else if (!(*pte & PAGEWIRED)) {
#if 0
            pg = pagefind(adr);
            pagerm(pg);
#endif
            k_physmem.pagestat.nmap--;
        } else {
//                kprintf("UNWIRE\n");
            k_physmem.pagestat.nwire--;
        }
        pagefreephys((void *)adr);
        *pte = 0;
        pte++;
    }

    return;
}

FASTCALL
void
vmpagefault(uintptr_t pid, uintptr_t adr, m_ureg_t error)
{
    uintptr_t     *pte = (uintptr_t *)&_pagetab + vmpagenum(adr);
    uintptr_t      flg = (uintptr_t)*pte & (PAGEFLTFLGMASK | PAGESYSFLAGS);
    struct vmpage *page = NULL;
    long           qid;

    kprintf("PAGEFAULT: pid == %lx, adr == %lx, error == %lx\n",
            (long)pid, (long)adr, (long)error);
    if (!(adr & ~(PAGEFLTADRMASK | PAGESYSFLAGS))) {
        page = pageallocphys();
        if (page) {
            page->nref++;
            if (flg & PAGEWIRED) {
                k_physmem.pagestat.nwire++;
            } else {
                k_physmem.pagestat.nmap++;
                page->nmap++;
                if (!(adr & PAGEWIRED)) {
                    qid = pagecalcqid(page);
                    vmlkpage(&k_physmem.lrutab[qid].lk);
                    deqpush(page, &k_physmem.lrutab[qid].next);
                    vmunlkpage(&k_physmem.lrutab[qid].lk);
                }
            }
            *pte = adr | flg | PAGEPRES;
        }
#if (PAGEDEV)
    } else if (!(page & PAGEPRES)) {
        // pageout();
        page = vmpagein(page);
        if (page) {
            vmlkpage(&page->lk);
            page->nmap++;
            qid = pagecalcqid(page);
            vmlkpage(&k_physmem.lrutab[qid].lk);
            deqpush(page, &k_physmem.lrutab[qid].next);
            vmunlkpage(&k_physmem.lrutab[qid].lk);
            vmunlkpage(&page->lk);
        }
#endif
    }
#if 0
    if (page) {
        pageaddset(page);
    }
#endif

    return;
}

#if 0
void
vmseekdev(uintptr_t dev, uint64_t ofs)
{
    devseek(k_vmdevtab[dev], ofs, SEEK_SET);
}

uintptr_t
vmpagein(uintptr_t adr)
{
    uintptr_t      pageid = vmpagenum(adr);
    uintptr_t      blk = vmblkid(pageid);
    struct vmpage *page = pagefind(adr);
    void          *data;

    vmlkpage(&k_vmdevlktab[dev], MEMPID);
    vmseekdev(dev, blk * PAGESIZE);
    page->nmap++;
//    data = pageread(dev, PAGESIZE);
    vmunlkpage(&k_vmdevlktab[pagedev], MEMPID);
}

void
vmpagefree(uintptr_t adr)
{
    ;
}
#endif /* 0 */

