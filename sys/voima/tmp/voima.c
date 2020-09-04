#include <mach/param.h>

#define L1_ICACHE_SLOTS     64

static int8_t       l1icache[L1_ICACHE_SLOTS][MACH_CL_SIZE];
static struct tagcl l1icachetags[L1_ICACHE_SLOTS][2];

static void
voimafetchl1i(uintptr_t cladr)
{
    uintptr_t   hash;

    cladr &= ~(MACH_CL_SIZE - 1);
    hash = m_hashadr(cladr);
    cladr |= VOIMA_CL_VALID_TAG;
    l1icachetags[hash][1].desc = l1icachetags[hash][0].desc;
    l1icachetags[hash][0].desc = cladr;
    memcpy(&l1icache[hash][0], (void *)cladr, MACH_CL_SIZE);

    return;
}

static int32_t
voimareadl1i(uintptr_t adr)
{
    uintptr_t   cl = adr & ~(MACH_CL_SIZE - 1);
    uintptr_t   w = adr & (MACH_CL_SIZE - 1);
    uintptr_t   hash = m_hashadr(cl);
    int32_t     word;

    if (!(l1icachetags[hash].desc & VOIMA_CL_VALID_TAG)) {
        voimafetchl1i(cl);
    }
    word = *(int32_t *)&l1icache[hash][w];
    
    return word;
}

