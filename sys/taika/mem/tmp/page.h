#ifndef __KERN_MEM_PAGE_H__
#define __KERN_MEM_PAGE_H__

#include <kern/conf.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/asm.h>
#include <zero/trix.h>
//#include <mt/tktlk.h>
#include <kern/types.h>
#include <kern/perm.h>
#include <kern/time.h>
#include <kern/cpu.h>
#include <kern/mem/vm.h>

#define DEQ_SINGLE_TYPE
#define DEQ_TYPE struct vmpage
#include <zero/deq.h>

#define PAGENODEV (-1)
#define PAGENOOFS (-1)

/* page ID */
#define pagenum(adr)  ((adr) >> PAGESIZELOG2)
//#define swapblknum(sp, pg) ((pg) - (sp)->pgtab)
#define pageadr(pg, pt)                                                 \
    ((!(pg)) ? NULL : ((void *)(((pg) - (pt)) << PAGESIZELOG2)))

/* index into table of LRU-queues */
#if (WORDSIZE == LONGSIZE)
#define pagecalcqid(pg)   max(PTRBITS - 1, lzerol(pg->nmap))
#elif (WORDSIZE == 8)
#define pagecalcqid(pg)   max(PTRBITS - 1, lzeroll(pg->nmap))
#endif

/* working sets */
#if 0
#define pageinset(pg)  (vmsetmap[pagenum((pg)->adr)])
#define pageaddset(pg) (vmsetmap[pagenum((pg)->adr)] = k_curpid)
extern pid_t           vmsetmap[NPAGEPHYS];
#endif
#define pageinset(pg)  bitset(vmsetbitmap, pagenum((pg)->adr))
#define pageaddset(pg) setbit(vmsetbitmap, pagenum((pg)->adr))
#define pageclrset(pg) clrbit(vmsetbitmap, pagenum((pg)->adr))

typedef uint64_t swapoff_t;

#define PAGEDEVMASK        ((UINT64_C(1) << PAGESIZELOG2) - 1)
#define swapsetblk(u, blk) ((u) |= (blk) << PAGESIZELOG2)
#define swapsetdev(u, dev) ((u) |= (dev))
#define swapblkid(adr)     ((adr) >> PAGESIZELOG2)
#define swapdevid(adr)     ((adr) & PAGEDEVMASK)
struct swapdev {
    m_atomic_t     lk;
    swapoff_t      npg;
    swapoff_t     *pgmap;
    struct vmpage *pgtab;
    struct vmpage *freeq;
};

extern m_ureg_t        pageinitphyszone(uintptr_t base, struct vmpage *zone,
                                        m_ureg_t nb);
extern m_ureg_t        pageinitphys(uintptr_t base, m_ureg_t nb);
extern struct vmpage * pageallocphys(void);
extern void            pagefreephys(void *adr);
#if 0
void                   pagefree(void *adr);
void                   swapfree(uintptr_t adr);
#endif

#endif /* __KERN_MEM_PAGE_H__ */

