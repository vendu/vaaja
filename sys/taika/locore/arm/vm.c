#include <stdint.h>
#include <zero/trix.h>
#include <zero/param.h>
#include <zero/cdecl.h>
#include <kern/unit/arm/link.h>
#include <kern/unit/arm/vm.h>

uint32_t *vml1pagetab[NL1PAGETAB] ALIGNED(16384);
uint32_t *vml2pagetab = (uint32_t *)&_l2pagetab;

void
segmap(uint32_t adr, uint32_t npg, uint32_t flg)
{
    uint32_t *pte;

    pte = &vml1pagetab[pagetabid(adr)][pageid(adr)];
    while (npg--) {
        /* map 4 kilobyte pages with 4 consecutive identical PTEs */
#if defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__)
        pte[0] = pte[1] = pte[2] = pte[3] = adr | flg;
#else
        *pte = adr | flg;
#endif
        adr += PAGESIZE;
        pte += 4;
    }

    return;
}

void
vminit(void)
{
    uint8_t *ptr = (uint8_t *)vml2pagetab;
    uint32_t reg;
    long     l;

    for (l = 0 ; l < NL1PAGETAB ; l++) {
        vml1pagetab[l] = (uint32_t *)((uint32_t)ptr | PAGEBITS);
        ptr += PAGESIZE;
    }
#if 0
    _pagetabl1 = &vml1pagetab;
    _pginit();
#endif
    /* copy page table address to cp15 */
    __asm__ __volatile__ ("mcr p15, #0, %0, c2, c0, #0\n"
                          :
                          : "r" (vml1pagetab)
                          : "memory");
    /* set access control to supervisor */
    __asm__ __volatile__ ("mcr p15, #0, %0, c1, c0, #0\n"
                          :
                          : "r" (~0));
    /* enable the MMU */
    __asm__ __volatile__ ("mcr p15, #0, %0, c1, c0, #0\n"
                          : "=r" (reg)
                          :
                          : "cc");
    reg |= 0x01;
    __asm__ __volatile__ ("mcr p15, #0, %0, c1, c0, #0\n"
                          :
                          : "r" (reg)
                          : "cc");

    return;
}

