#ifndef __MT_SPINRW_H__
#define __MT_SPINRW_H__

/* velho read-write spinlocks */

/* REFERENCE: https://jfdube.wordpress.com/2014/01/03/implementing-a-recursive-read-write-spinlock/ */

/*
 * NOTES
 * -----
 * - initialise spinrw-locks with spininit() from <mt/spin.h>
 */

#if (LONGSIZE == 8)
#define SPINRWWRBIT             (UINT64_C(1) << 63)
#elif (LONGSIZE == 4)
#define SPINRWWRBIT             (UINT32_C(1) << 31)
#endif
#define SPINRWCNTMASK           (~SPINWRWRBIT)

#define MTSPINRW_FREE           (-1L)
#define MTSPINRWREC_DEFVAL      { MTSPININITVAL, MTSPINRW_FREE, 0 }
/* structure for recursive locks */
typedef struct spinrw {
    volatile long lk;   // lock value
    long          thr;  // writer thread ID
    long          rec;  // recursion depth
} mtspinrwrec;

static __inline__ void
mttryspinrd(volatile long *sp)
{
    volatile long               old;
    volatile long               val;

    do {
        while (*sp & SPINRWWRBIT) {
            m_waitspin();
        }
        old = *sp & SPINRWCNTMASK;
        val = old + 1;
        if (m_cmpswap(sp, old, val)) {

            return;
        }
    } while (1);

    return;
}

static __inline__ void
mtspinunlkrd(volatile long *sp)
{
    m_atominc(*sp);

    return;
}

static __inline__ void
mtlkspinwr(volatile long *sp)
{
    volatile long               old;
    volatile long               val;

    do {
        while (*sp & SPINRWWRBIT) {
            thryield();
        }
        old = *sp & SPINRWCNTMASK;
        val = old | SPINRWWRBIT;

        if (m_cmpswap(sp, old, val) == old) {
            while (*sp & SPINRWCNTMASK) {
                m_waitspin();
            }
        }
    } while (1);

    return;
}

static __inline__ void
mtunlkspinwr(volatile long *sp)
{
    assert(*sp == SPINRWWRBIT);
    *sp = MTSPININITVAL;
    m_endspin();

    return;
}

static __inline__ void
mtlkspinrwrecwr(mtspinrwrec *spin)
{
    long                        thr = thrid();

    if (spin->thr == thr) {
        mtlkspinwr(&spin->lk);
        assert(!spin->rec);
        spin->thr = thr;
    }
    spin->rec++;
}

static __inline__ void
mtunlkspinrwrecwr(mtspinrwrec *spin)
{
    long                        thr = thrid();

    assert(thr == spin->thr);
    spin->rec--;
    if (!spin->rec) {
        spin->thr = MTSPINRW_FREE;
        mtunlkspinwr(&spin->lk);
    }
}

#endif /* __MT_SPINRW_H__ */

