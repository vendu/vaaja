#include <mach/types.h>
#include <taika/mem/zone.h>

extern struct memzone k_memzonetab[MEM_ZONES];

void
meminit(m_size_t base, m_size_t nbphys, m_size_t nbvirt)
{
    m_size_t lim = min(KERNVIRTBASE, nbphys);
    m_size_t end;
    m_size_t adr;

    vminitphys((uintptr_t)base, lim - base);
    end = min(KERNVIRTBASE, nbvirt);
    if (end > lim) {
        vminitvirt((void *)lim, end - lim, PAGEWRITE);
    }
#if defined(__x86_64__) || defined(__amd64__)
#error implement x86-64 memory management
#endif

    return;
}

