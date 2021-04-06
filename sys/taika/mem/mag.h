#ifndef TAIKA_MEM_MAG_H
#define TAIKA_MEM_MAG_H

#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>

#define mempop(mp)       ((mp)->ptab[((mp)->ndx)++])
#define mempush(mp, ptr) ((mp)->ptab[--((mp)->ndx)] = (ptr))
#define memmagempty(mp)  ((mp)->ndx == (mp)->nmax)
#define memmagfull(mp)   (!(mp)->ndx)

#define __STRUCT_MEMMAG_PTAB_SIZE                                       \
    ((1UL << (MEMSLABSHIFT - MEMMINSHIFT)) * sizeof(void *))
#if defined(MEMPARANOIA)
#if (MEMSLABSHIFT - MEMMINSHIFT < (LONGSIZELOG2 + 3))
#define __STRUCT_MEMMAG_BMAP_SIZE sizeof(long)
#else
#define __STRUCT_MEMMAG_BMAP_SIZE                                       \
    (1UL << (MEMSLABSHIFT - MEMMINSHIFT - 3))
#endif
#else
#define __STRUCT_MEMMAG_BMAP_SIZE 0
#endif /* defined(MEMPARANOIA) */
struct memmag {
    m_atomic_t     lk;
    uintptr_t      base;
    volatile long  info;
    struct memmag *prev;
    struct memmag *next;
#if defined(MEMPARANOIA) && (MEMSLABSHIFT - MEMMINSHIFT < (LONGSIZELOG2 + 3)) && 0
    unsigned long   bmap;
#endif
    volatile long   ndx;
    volatile long   nmax;
    void           *ptab[1UL << (MEMSLABSHIFT - MEMMINSHIFT)];
#if defined(MEMPARANOIA) &&  !(MEMSLABSHIFT - MEMMINSHIFT < (LONGSIZELOG2 + 3)) && 0
    uint8_t         bmap[__STRUCT_MEMMAG_BMAP_SIZE];
#endif /* defined(MEMPARANOIA) */
};

#if 0

#define memgetmag(ptr, pool)                                            \
    (((ptr)                                                             \
      ? ((struct memmag *)(pool)->blktab + memgetmagnum(ptr, pool))     \
      : NULL))

#endif

#define memclrmaginfo(mp)                                               \
    ((mp)->info = 0)
#define memclrmaglist(mp)                                               \
    ((mp)->info &= ~MEMLISTMASK)
#define memsetmaglist(mp, list)                                         \
    (memclrmaglist(mp), (mp)->info |= (list))
#define memmaggetlist(mp)                                               \
    ((mp)->info & MEMLISTMASK)
#define memmagisfree(mp)                                                \
    (!((mp)->info & MEMFREE))
#define memsetmagfree(mp)                                               \
    ((mp)->info |= MEMFREE)
#define memclrmagfree(mp)                                               \
    ((mp)->info &= ~MEMFREE)
#define memsetmagflg(mp, flg)                                           \
    ((mp)->info |= (flg))
#define memclrmagflg(mp)                                                \
    ((mp)->info &= ~MEMFLGBITS)
#define memclrmaglink(mp)                                               \
    ((mp)->prev = (mp)->next = NULL)
#define memsetmagprev(mp, hdr)                                          \
    ((mp)->prev = hdr)
#define memsetmagnext(mp, hdr)                                          \
    ((mp)->next = hdr)
#define memmaggetprev(mp)                                               \
    ((mp)->prev)
#define memmaggetnext(mp)                                               \
    ((mp)->next)
#define memclrmagprev(mp)                                               \
    ((mp)->prev = NULL)
#define memclrmagnext(mp)                                               \
    ((mp)->next = NULL)

#endif /* TAIKA_MEM_MAG_H */

