#include <zero/param.h>
#include <kern/x86-64/vm.h>

uint64_t vmkpml4phys;
uint64_t vmkpdpphys;
uint64_t vmnkernpagetab;

#if 0
extern uint64_t kernpagetab4[NPAGETAB];
extern uint64_t kernpagetab3[NPAGETAB];
extern uint64_t kernpagetab2[NPAGETAB];
extern uint64_t kernpagetab1[NPAGETAB];

void
vmmapseg(uint64_t *pagemap, uint64_t virt, uint64_t phys, uint64_t lim,
         uint64_t flg)
{
    uint64_t *pt4tab = pagemap;
    uint64_t *pt3tab;
    uint64_t *pt2tab;
    uint64_t *pt1tab;
    long      pt4ndx = vmpt4num(virt);
    long      pt3ndx = vmpt3num(virt);
    long      pt2ndx = vmpt2num(virt);
    long      pt1ndx = vmpt1num(virt);

    n = rounduppow2(lim - virt, PAGESIZE) >> PAGESIZELOG2;
    while (virt < lim) {
        for ( ; pt4ndx < NPAGETAB ; pt4ndx++) {
            pt3tab = (uint64_t *)pt4tab[pt4ndx];
            for ( ; pt3ndx < NPAGETAB ; pt3ndx++) {
                pt2tab = (uint64_t *)pt3tab[pt3ndx];
                for ( ; pt2ndx < NPAGETAB ; pt2ndx++) {
                    pt1tab = (uint64_t *)pt2tab[pt2ndx];
                    for ( ; pt1ndx < NPAGETAB ; pt1ndx++) {
                        pt1tab[pt1ndx] = virt | flg;
                        virt += PAGESIZE;
                        pt1ndx++;
                    }
                    pt2ndx++;
                }
                pt3ndx++;
            }
            pt4ndx++;
        }
    }
}

void
vminitlong(void *pagemap)
{
    uint64_t *pt4tab = pagemap;
    uint64_t *pt3tab = kernpagedirptrtab;
    uint64_t *pt2tab = kernpagedirtab;
    uint64_t *pagetab = kernpagetab;

    /* set page structures up to point to each other */
    pt4tab[0] = (uint64_t)pmdtab | PAGEPRES | PAGEWRITE;
    pmdtab[0] = (uint64_t)dirtab | PAGEPRES | PAGEWRITE;
    dirtab[0] = (uint64_t)pagetab | PAGEPRES | PAGEWRITE;
    pginitlong();
}
#endif /* 0 */

