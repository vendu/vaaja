#ifndef __MT_MTX_H__
#define __MT_MTX_H__

#include <mt/conf.h>

#if defined(MT_ZERO_MUTEX)

/* velho mutex locks */

#define MT_MUTEX           1
#if !defined(MT_FAST_MUTEX)
#define MT_FAST_MUTEX          1
#endif

typedef volatile m_atomic_t mtmtx;
typedef volatile m_atomic_t mtfmtx;

/*
 * Special thanks to Matthew 'kinetik'
 * Gregan for help with the mutex code.:)
 */

#define fmtxinit(mp)    (*(mp) = MT_MUTEX_INITVAL)
#define fmtxfree(mp) /* no-op */

#define MT_MUTEX_INITVAL    0
#define MT_MUTEX_LKVAL      1
#define MT_MUTEX_CONTVAL    2
//#define MT_FAST_MUTEX_INITVAL   0
//#define MT_FAST_MUTEX_LKVAL     1

static C_INLINE void
mtinitfmtx(mtfmtx *mp)
{
    m_membar();         // full memory barrier
    *mp = MT_MUTEX_INITVAL;  // lazy-write

    return;
}

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static C_INLINE long
mttrylkfmtx(mtfmtx *mp)
{
    m_atomic_t  res = 0;

    if (*mp == MT_MUTEX_INITVAL) {
        res = m_cmpswap(mp, MT_MUTEX_INITVAL, MT_MUTEX_LKVAL);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static C_INLINE void
mtlkfmtx(mtfmtx *mp)
{
    m_atomic_t  res = 0;

    do {
        while (*mp != MT_MUTEX_INITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(mp, MT_MUTEX_INITVAL, MT_MUTEX_LKVAL);
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
mtunlkfmtx(mtfmtx *mp)
{
    m_membar();         // full memory barrier
    *mp = MT_MUTEX_INITVAL;  // lazy-write
    m_endspin();        // signal wakeup-event

    return;
}

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static C_INLINE long
mttrylkrecfmtx(mtfmtx *mp)
{
    m_atomic_t  res = 0;
    m_atomic_t  id = (m_atomic_t)mtthrself();

    if (*mp == MT_MUTEX_INITVAL) {
        res = m_cmpswap(mp, MT_MUTEX_INITVAL, id);
    }
    if (!res) {
        res = (*mp == id);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static C_INLINE void
mtlkrecfmtx(mtfmtx *mp)
{
    m_atomic_t  res = 0;
    m_atomic_t  id = (m_atomic_t)mtthrself();

    do {
        while (*mp != MT_MUTEX_INITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(mp, MT_MUTEX_INITVAL, MT_MUTEX_LKVAL);
        if (!res) {
            res = (*mp == id);
        }
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
mtunlkrecfmtx(mtfmtx *mp)
{
    m_membar();         // full memory barrier
    *mp = MT_MUTEX_INITVAL;  // lazy-write
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* defined(MT_ZERO_MUTEX) */

#endif /* __MT_MTX_H__ */

