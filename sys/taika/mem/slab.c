#include <stddef.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <zero/trix.h>
#include <mt/mtx.h>
#include <kern/util.h>
#include <kern/mem/vm.h>
#include <kern/mem/mem.h>
#include <kern/mem/zone.h>

#define MEMDIAG 0

struct memzone k_memzonetab[MEM_ZONES];

/*
 * zero slab allocator
 * -------------------
 * - slabs are power-of-two-sizes
 * - slabs are combined to and split from bigger ones on demand;
 *   free regions are kept as big as possible.
 */

#if !(__KERNEL__) && (MEMDIAG)
void
memdiag(struct memzone *zone)
{
    long            bktid;
    struct memslab *slab1;
    struct memslab *slab2;
    struct memslab *null = NULL;

    memlk(&memphyszonelk);
    for (bktid = 0 ; bktid < PTRBITS ; bktid++) {
            slab1 = (struct memslab **)zone->tab[bktid];
             if (slab1) {
                if (memgetprev(slab1)) {
#if (__KERNEL__)
                    fprintf(stderr, "DIAG: non-NULL prev on head of list %ld\n",
                            bktid);
#endif
                    memprint(slab1);

                    *null = *slab1;
                }
                if (memslabgetbkt(slab1) != bktid) {
#if (__KERNEL__)
                    fprintf(stderr, "DIAG: invalid bucket ID on head of list %ld\n",
                            bktid);
#endif
                    memprint(slab1);

                    *null = *slab1;
                }
                slab2 = memgetnext(slab1);
                while (slab2) {
                    if (memgetprev(slab2) != slab1) {
#if (__KERNEL__)
                        fprintf(stderr, "DIAG: invalid prev on list %ld\n",
                                bktid);
#endif
                        memprint(slab1);

                        *null = *slab1;
                    }
                    if (memslabgetbkt(slab2) != bktid) {
#if (__KERNEL__)
                        fprintf(stderr, "DIAG: invalid bucket ID on list %ld\n",
                                bktid);
#endif
                        memprint(slab1);

                        *null = *slab1;
                    }
                    slab1 = slab2;
                    slab2 = memgetnext(slab2);
                }
            }
        }
    }
    memunlk(&memphyszonelk);

    return;
}
#endif

/*
 * combine slab with bigger [previous- or next-address] ones if possible.
 */
long
slabcomb(struct memzone *zone, struct memmag *mag)
{
    long           ret  = 0;
    struct membkt *bkt = zone->tab;
    m_ureg_t       nblk = zone->nblk;
    long           bktid = memmaggetbkt(mag);
    long           bktid1 = bktid;
    long           bktid2 = bktid1;
    long           prev = 1;
    long           next = 1;
    intptr_t       ofs = 1UL << (bktid1 - MEMMINSHIFT);
    struct memmag *mag1;
    struct memmag *mag2;
    struct memmag *mag3;
    struct memmag *mag4;

    mag1 = mag;
    while ((prev) || (next)) {
        prev ^= prev;
        next ^= next;
        if (mag - (struct memmag *)zone->hdrtab >= ofs) {
            mag1 = mag - ofs;
            bktid2 = memmaggetbkt(mag1);
            if (bktid2 == bktid1 && memmagisfree(mag1)) {
                prev++;
                ret++;
                mag3 = memmaggetprev(mag1);
                mag4 = memmaggetnext(mag1);
                if ((mag3) && (mag4)) {
                    memsetmagnext(mag3, mag4);
                    memsetmagprev(mag4, mag3);
                } else if (mag3) {
                    memsetmagnext(mag3, mag4); // NULL
                } else if (mag4) {
                    memsetmagprev(mag4, mag3); // NULL
                    bkt[bktid1].list = mag4;
                } else {
                    bkt[bktid1].list = NULL;
                }
                memclrmaginfo(mag);
                memclrmaglink(mag);
//                bktid2++;
                bktid1++;
                ofs <<= 1;
            }
        }
        if (!mag1) {
            mag1 = mag;
        }
        if (mag1 + ofs < (struct memmag *)zone->hdrtab + nblk) {
            mag2 = mag1 + ofs;
            bktid2 = memmaggetbkt(mag2);
            if (bktid2 == bktid1 && memmagisfree(mag2)) {
                next++;
                ret++;
                mag3 = memmaggetprev(mag2);
                mag4 = memmaggetnext(mag2);
                if ((mag3) && (mag4)) {
                    memsetmagnext(mag3, mag4);
                    memsetmagprev(mag4, mag3);
                } else if (mag3) {
                    memsetmagnext(mag3, mag4); // NULL;
                } else if (mag4) {
                    memsetmagprev(mag4, mag3); // NULL
                    bkt[bktid1].list = mag4;
                } else {
                    bkt[bktid1].list = NULL;
                }
                memclrmaginfo(mag2);
                memclrmaglink(mag2);
                bktid2++;
                memclrmaginfo(mag1);
                memclrmaglink(mag1);
                memsetmagbkt(mag1, bktid2);
                memsetmagfree(mag1);
                bktid1 = bktid2;
                ofs <<= 1;
                mag1 = NULL;
            }
        }
        if (mag1) {
            mag = mag1;
        }
    }
    if (ret) {
        mag1 = bkt[bktid].list;
        memclrmaginfo(mag);
        memclrmaglink(mag);
        memsetmagbkt(mag, bktid1);
        memsetmagfree(mag);
        if (mag1) {
            memsetmagprev(mag1, mag);
            memsetmagnext(mag, mag1);
        }
        bkt[bktid1].list = mag;
    }
#if (__KERNEL__ && (MEMDIAG))
    memdiag(zone);
#endif

    return ret;
}

/*
 * - split slab into smaller ones to satisfy allocation request
 * - split of N to M gives us one free slab in each of M to N-1 and one to
 *   allocate in M
 * - caller has locked the bucket dest
 */
void
slabsplit(struct memzone *zone, struct memmag *mag, long dest)
{
    struct membkt *bkt = zone->tab;
    long           bktid = memmaggetbkt(mag);
    uint8_t       *ptr = memgetadr(mag, zone);
    struct memmag *mag1;
    struct memmag *mag2;
    size_t         sz = 1UL << bktid;

    ptr += 1UL << bktid;
    while (--bktid >= dest) {
        sz >>= 1;
        ptr -= sz;
        mag1 = memgetmag(ptr, zone);
        mag2 = bkt[bktid].list;
        memclrmaginfo(mag1);
        memclrmaglink(mag1);
        memsetmagbkt(mag1, bktid);
        memsetmagfree(mag1);
        if (mag2) {
            memsetmagprev(mag2, mag1);
            memsetmagnext(mag1, mag2);
        }
        bkt[bktid].list = mag1;
    }
    mag1 = bkt[dest].list;
    memclrmaginfo(mag);
    memclrmaglink(mag);
    memsetmagbkt(mag, dest);
    memsetmagfree(mag);
    if (mag1) {
        memsetmagprev(mag1, mag);
        memsetmagnext(mag, mag1);
    }
    bkt[dest].list = mag;
#if (__KERNEL__ && (MEMDIAG))
    memdiag(zone);
#endif

    return;
}

/*
 * allocate mag; split from larger ones if necessary.
 */
void *
slaballoc(struct memzone *zone, m_ureg_t nb, long flg)
{
    struct membkt *bkt = zone->tab;
    long           bktid1 = memcalcbkt(nb);
    long           bktid2 = bktid1;
    uint8_t       *ptr = NULL;
    struct memmag *mag1;
    struct memmag *mag2;

    memlk(&zone->lk);
    mag1 = bkt[bktid1].list;
    if (!mag1) {
        while (!mag1 && ++bktid2 < PTRBITS) {
            mag1 = zone->tab[bktid2].list;
            if (mag1) {
                mag2 = memmaggetnext(mag1);
                if (mag2) {
                    memclrmagprev(mag2);
                }
                zone->tab[bktid2].list = mag2;
                memclrmaglink(mag1);
                slabsplit(zone, mag1, bktid1);
                mag1 = zone->tab[bktid1].list;
            }
        }
    }
    if (mag1) {
        mag2 = memmaggetnext(mag1);
        if (mag2) {
            memclrmagprev(mag2);
        }
        zone->tab[bktid1].list = mag2;
        memclrmagfree(mag1);
        memclrmaglink(mag1);
        memsetmagflg(mag1, flg);
        ptr = memgetadr(mag1, zone);
    }
    memunlk(&zone->lk);
#if (__KERNEL__ && (MEMDIAG))
    memdiag(zone);
#endif

    return ptr;
}

/*
 * free slab; combine to form bigger ones if possible.
 */
void
slabfree(struct memzone *zone, void *ptr)
{
    struct membkt *bkt = zone->tab;
    struct memmag *mag1 = memgetmag(ptr, zone);
    long           bktid = memmaggetbkt(mag1);
    struct memmag *mag2;

    memlk(&zone->lk);
#if !defined(MEMTEST)
    vmfreephys(ptr, 1UL << bktid);
#endif
    memsetmagfree(mag1);
    if (!slabcomb(zone, mag1)) {
        mag2 = bkt[bktid].list;
        memclrmaglink(mag1);
        if (mag2) {
            memsetmagprev(mag2, mag1);
        }
        memsetmagnext(mag1, mag2);
        bkt[bktid].list = mag1;
    }
    memunlk(&zone->lk);
#if (__KERNEL__ && (MEMDIAG))
    memdiag(zone);
#endif

    return;
}

