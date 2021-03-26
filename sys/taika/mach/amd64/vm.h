#ifndef __UNIT_X86_64_VM_H__
#define __UNIT_X86_64_VM_H__

/* REFERENCE: http://fxr.watson.org/fxr/source/amd64/amd64/pmap.c */

#include <kern/types.h>

/*
 * Virtual addresses of things.  Derived from the page directory and
 * page table indexes from pmap.h for precision.
 *
 * 0x0000000000000000 - 0x00007fffffffffff   user map
 * 0x0000800000000000 - 0xffff7fffffffffff   does not exist (hole)
 * 0xffff800000000000 - 0xffff804020100fff   recursive page table (512GB slot)
 * 0xffff804020101000 - 0xfffff7ffffffffff   unused
 * 0xfffff80000000000 - 0xfffffbffffffffff   4TB direct map
 * 0xfffffc0000000000 - 0xfffffdffffffffff   unused
 * 0xfffffe0000000000 - 0xffffffffffffffff   2TB kernel map
 *
 * Within the kernel map:
 *
 * 0xffffffff80000000                        KERNBASE
 */

#if defined(__x86_64__) || defined(__amd64__)

#include <stdint.h>
#include <zero/param.h>
#include <zero/mtx.h>
#include <kern/unit/ia32/vm.h>

#if 0
#define PAGEPATNOCACHE 0 // uncached
#define PAGEPATWRCOMB  1 // write-combining
#define PAGEPATWRTHRU  2 // write-through
#define PAGEPATWRPROT  3 // write-protected
#define PAGEPATWRBACK  4 // write-back
#define PAGEPATUCMINUS 5 // uncached, but can be MTRR-overridden
#endif

extern uint64_t kernpagemapl4tab[512];

#define NADRBIT ADRBITS

static INLINE uint64_t
vmsignadr(void *adr)
{
    int64_t  val = (int64_t)adr;
    int64_t  sign = INT64_C(1) << (NADRBIT - 1);
    int64_t  lobits = (INT64_C(1) << NADRBIT) - 1;
    int64_t  res;

    val &= lobits;
    res = (val ^ sign) - sign;

    return res;
}

#define vmpgdnum(adr)  (((uint64_t)(adr) >> 39) & 0x1ff)
#define vmpmdnum(adr)  (((uint64_t)(adr) >> 30) & 0x1ff)
#define vmptenum(adr)  (((uint64_t)(adr) >> 21) & 0x1ff)
#define vmpagenum(adr) (((uint64_t)(adr) >> 12) & 0x1ff)
#define vmpageofs(adr) ((uint64_t)(adr) & (PAGESIZE - 1))

#if !defined(__ASSEMBLER__)
#define PAGENOEXEC  UINT64_C(0x8000000000000000U)
#endif

#define EXTPAGEREAD       0x00000001
#define EXTPAGEWRITE      0x00000002
#define EXTPAGEEXEC       0x00000004
#define EXTPAGENOPAT      0x00000040
#define EXTPAGESUPER      0x00000080
#define EXTPAGEREF        0x00000100
#define EXTPAGEDIRTY      0x00000200
#define EXTPAGEMEMTYPE(x) ((x) << 3)

#define NPAGETAB          512 // # of page-structure table entries
#define PTSHIFT           12
#if !defined(__ASSEMBLER__)
#define VMPAGEMASK        UINT64_C(0xfffffffffffff000)
#if (ADRBITS == 48)
#define VMPHYSMASK        UINT64_C(0x0000ffffffffffff)
#elif ADRBITS == 52)
#define VMPAGEMASK        UINT64_C(0x000ffffffffff000)
#elif (ADRBITS == 56)
#define VMPHYSMASK        UINT64_C(0x00ffffffffffffff)
#elif (ADRBITS == 64)
#define VMPHYSMASK        UINT64_C(0xffffffffffffffff)
#endif
#endif

#define PAGEMANAGED       PAGESYS2
#undef VMPAGEMASK
#define VMPAGEMASK        (VMPHYSMASK & ~((1U << PAGESIZELOG2) - 1))

#define PAGEPMDCACHE      (PAGESUPERPDE | PAGEWRITETHRU | PAGENOCACHE)
#define PAGEPTECACHE      (PAGESUPERPTE | PAGEWRITETHRU | PAGENOCACHE)

/* identical settings for 2MB (PDE) and 4KB (PTE) page mappings */
#define PAGEPTEPROMOTE                                                  \
    (PAGENOEXEC | PAGEMANAGED | PAGEWIRED | PAGEGLOBAL | PAGEPTECACHE   \
     | PAGEDIRTY | PAGEREF | PAGEUSER | PAGEWRITE | PAGEPRES)

#define PAGEFLTPROTBAD    0x01  // violation vs. not present
#define PAGEFLTWRITE      0x02  // write fault
#define PAGEFLTUSER       0x04  // access from user-mode
#define PAGEFLTPTEBAD     0x08  // reserved PTE-field non-zero
#define PAGEFLTINSTFETCH  0x10  // instruction fetch fault

#if defined(AMD64_NPT_AWARE)
//#undef PAGESYS1
#undef PAGEGLOBAL
#undef PAGEREF
#undef PAGEDIRTY
#undef PAGESUPERPTE
#undef PAGEPTECACHE
#undef PAGESUPERPDE
#undef PAGEPDECACHE
#undef PAGEWRITE
#undef PAGEPRES
#endif /* defined(AMD64_NPT_AWARE) */

#define vmpageid(adr) ((uintptr_t)(adr) >> PAGESIZELOG2)

/* level #1 */
#define PTESPERPAGE (PAGESIZE / sizeof(pte_t))
#define PTESHIFT    12
/* levvel #2 */
#define PDESPERPAGE (PAGESIZE / sizeof(pde_t))
#define PDESHIFT    21
/* level #3 */
#define PMDSPERPAGE (PAGESIZE / sizeof(pmd_t))
#define PMDSHIFT    30
/* level #4 */
#define PGDSPERPAGE (PAGESIZE / sizeof(pgd_t))
#define PGDSHIFT    39

#define kernvirtadr(l4, l3, l2, l1)                                     \
    (((uint64_t)-1 << (ADRBITS - 1))                                    \
     | ((uint64_t)(l4) << PGDSHIFT)                                     \
     | ((uint64_t)(l3) << PMDSHIFT)                                     \
     | ((uint64_t)(l2) << PDESHIFT)                                     \
     | ((uint64_t)(l1) << PTESHIFT))

#define usrvirtadr(l4, l3, l2, l1)                                      \
    (((uint64_t)(l4) << PGDSHIFT)                                       \
     | ((uint64_t)(l3) << PMDSHIFT)                                     \
     | ((uint64_t)(l2) << PDESHIFT)                                     \
     | ((uint64_t)(l1) << PTESHIFT))

#define KERNPGDS     4                         // # of kernel PGD slots
#define USERPGDS     (PGDSPERPAGE / 2)         // # of userland PGD pages
#define USERPGDPAGES (USERPGDS * PMDPAGES)     // # of userland PGD pages
#define USERPGDSLOTS (USERPMDPAGES * PGDPAGES) // # of userlaND PD entries

#define PGDMAPS      8                         // PGD entries for direct map
#define PGDRECMAP    (NPGDPERPAGE / 2)         // index of recursive PGD mapping

#define KERNPGDBASE (PGDSPERPAGE - KERNPGDS)   // KVM at highest addresses
#define PGDSPERMAP  rounddown(KERNPGDBASE - NPGDMAP, NPGDMAP) // below KVM

#define KERNPGDS    (NPGDPERPAGE - 1)
#define KERNPGD     (NPGDPERPAGE - 2)          // kernel base at -2GB

#if defined(__KERNEL__)

#define PTEMAPADR    kernvirtadr(PGDRECMAP, 0, 0, 0)
#define PDEMAPADR    kernvirtadr(PGDRECMAP, PGDRECMAP, 0, 0)
#define PMDMAPADR    kernvirtadr(PGDRECMAP, PGDRECMAP, PGDRECMAP, 0)
#define PGDMAPADR    kernvirtadr(PGDRECMAP, PGDRECMAP, PGDRECMAP, PGDRECMAP)
#define PGDMAPADR    (PGDMAPADR + (PGDRECMAP * sizeof(pgd_t)))

#define PTEMAP       ((pte_t *)PTMAPADR)
#define PDEMAP       ((pde_t *)PDMAPADR)
#define PMDMAP       ((pmp_t *)PDPMAPADR)
#define PGDMAP       ((pgd_t *)PGDMAPADR)
#define PGDPDEMAP    ((pde_t *)PGDMAPADR)

#define vmadrtophys(virt) pmap_kextract(((vm_offset_t)(virt)))

static __inline__
pte_t *
vmadrtopte(uintptr_t adr)
{
    uintptr_t mask = (UINT64_C(1)
                      << (NPTESHIFT + NPDESHIFT + NPMDSHIFT + NPGDSHIFT - 1));

    return (PTEMAP + ((adr >> PAGESHIFT) & mask));
}

static __inline__
pde_t *
vmadrtopde(uintptr_t adr)
{
    uintptr_t mask = (UINT64_C(1) << (NPDESHIFT + NPMDSHIFT + NPGDSHIFT - 1));

    return (PDEMAP + ((adr >> PDRSHIFT) & mask));
}

#define pteloadstore(ptep, pte) m_swap(ptep, pte)
#define pteloadclear(ptep)      m_swap(ptep, 0)
#define ptestore(ptep, pte)     (*(uintptr_t *)(ptep) = (uintptr_t)(pte))
#define pteclear(ptep)          pte_store(ptep, 0)
#define pdestore(pdep, pde)     pte_store(pdep, pde)

#endif /* defined(__KERNEL__) */

#define PMAP_PT_X86 0   // regular x86 page tables
#define PMAP_EPT    1   // Intel's nested page tables
#define PMAP_PT_RVI 2   // AMD's nested page tables

struct vmpage {
    struct vmpage *list;
    int            gen;
    int            pat;
};

#define PMAP_PCID_NONE    0xffffffff
#define PMAP_PCID_KERN    0
#define PMAP_PCID_OVERMAX 0x1000

struct vmpmappcids {
    uint32_t pcid;
    uint32_t gen;
};

#if 0
struct vmpagemap {
    pde_t *dir; // page directory address
};
#endif

struct vmmapentry {
    void              *adr;
    struct vmmapentry *next;
};

struct vmpagemap {
    volatile long  lk;
    pgde_t        *pgdtab;
    uint64_t       cr3;
};

#endif /* __x86_64__ || __amd64__ */

#define NVMBITWORDS 3
#define NVMMAPQUEUE 168
struct vmchunk {
    struct vmpagemap  *pagemap;
    struct vmchunk    *list;
    struct vmchunk    *lruqueue;
    uint64_t           bitmap[_NPCM];  /* bitmap; 1 = free */
    struct vmmapentry  mapqueue[_NPCPV];
};

#endif /* __UNIT_X86_64_VM_H__ */

