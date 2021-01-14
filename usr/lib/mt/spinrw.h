#ifndef __MT_SPINRW_H__
#define __MT_SPINRW_H__

#include <mt/conf.h>

#if defined(MT_SPINLOCKS)

#include <mach/param.h>

/* velho read-write spinlocks */

/* REFERENCE: https://jfdube.wordpress.com/2014/01/03/implementing-a-recursive-read-write-spinlock/ */

/*
 * NOTES
 * -----
 * - initialise spinrw-locks with spininit() from <mt/spin.h>
 */

#if (MACH_LONG_SIZE == 8)
#define SPINWRITEBIT            (INT64_C(1) << 63)
#elif (MACH_LONG_SIZE == 4)
#define SPINWRITEBIT            (INT32_C(1) << 31)
#endif
#define SPINCNTMASK             (~SPINWRITEBIT)

#define MTSPINRWFREE            (-1L)
#define MTSPINRWRECDEFVAL       { MTSPININITVAL, MTSPINRWFREE, 0 }
/* structure for recursive locks */
typedef struct __zenspinrwrec {
    volatile long lk;   // lock value
    long          thr;  // writer thread ID
    long          rec;  // recursion depth
} zenspinrwrec;

static __inline__ void
zentryspinrd(volatile long *sp)
{
    volatile long               old;
    volatile long               val;

    do {
        while (*sp & SPINWRITEBIT) {
            m_waitspin();
        }
        old = *sp & SPINCNTMASK;
        val = old + 1;
        if (m_cmpswap(sp, old, val)) {

            return;
        }
    } while (1);

    return;
}

static __inline__ void
zenspinunlkrd(volatile long *sp)
{
    m_atominc(sp);

    return;
}

static __inline__ void
zenlkspinwr(volatile long *sp)
{
    volatile long               old;
    volatile long               val;

    do {
        while (*sp & SPINWRITEBIT) {
            zenyieldthr();
        }
        old = *sp & SPINCNTMASK;
        val = old | SPINWRITEBIT;

        if (m_cmpswap(sp, old, val) == old) {
            while (*sp & SPINCNTMASK) {
                m_waitspin();
            }
        }
    } while (1);

    return;
}

static __inline__ void
zenunlkspinwr(volatile long *sp)
{
    assert(*sp == SPINWRITEBIT);

    *sp = MTSPININITVAL;
    m_endspin();

    return;
}

static __inline__ void
zenlkspinrwrecwr(zenspinrwrec *spin)
{
    long                        thr = thrself();

    if (spin->thr == thr) {
        zenlkspinwr(&spin->lk);
        assert(!spin->rec);
        spin->thr = thr;
    }
    spin->rec++;
}

static __inline__ void
zenunlkspinrwrecwr(zenspinrwrec *spin)
{
    long                        thr = thrself();

    assert(thr == spin->thr);
    spin->rec--;
    if (!spin->rec) {
        spin->thr = MTSPINRWFREE;
        zenunlkspinwr(&spin->lk);
    }
}

#endif /* defined(MT_SPINLOCKS)*/

#endif /* __MT_SPINRW_H__ */

