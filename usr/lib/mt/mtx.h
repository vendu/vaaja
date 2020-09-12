#ifndef __MT_MTX_H__
#define __MT_MTX_H__

/* velho mutex locks */

#include <mach/atomic.h>

#if !defined(__zen__) && 0
#define MTPTHREAD       1
#endif
#define pthread_self()  0

#define MTMTX           1
#if !defined(MTFMTX)
#define MTFMTX          1
#endif

//#include <mt/conf.h>
#include <stddef.h>
#include <stdint.h>
#if defined(MTPTHREAD)
#include <pthread.h>
#endif
#include <zero/cdefs.h>
#include <mach/param.h>
#include <mach/asm.h>

typedef volatile m_atomic_t mtmtx;
typedef volatile m_atomic_t mtfmtx;

/*
 * Special thanks to Matthew 'kinetik'
 * Gregan for help with the mutex code.:)
 */

#define fmtxinit(lp)    (*(lp) = MTMTXINITVAL)
#define fmtxfree(lp) /* no-op */

#define MTMTXINITVAL    0
#define MTMTXLKVAL      1
#define MTMTXCONTVAL    2
#define MTFMTXINITVAL   0
#define MTFMTXLKVAL     1

static C_INLINE void
mtinitfmtx(volatile m_atomic_t *lp)
{
    m_membar();         // full memory barrier
    *lp = MTMTXINITVAL;  // lazy-write

    return;
}

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static C_INLINE long
mttryfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;

    if (*lp == MTMTXINITVAL) {
        res = m_cmpswap(lp, MTMTXINITVAL, MTMTXLKVAL);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static C_INLINE void
mtlkfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;

    do {
        while (*lp != MTMTXINITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(lp, MTMTXINITVAL, MTMTXLKVAL);
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
mtunlkfmtx(volatile m_atomic_t *lp)
{
    m_membar();         // full memory barrier
    *lp = MTMTXINITVAL;  // lazy-write
    m_endspin();        // signal wakeup-event

    return;
}

#if !defined(__zen__)

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static C_INLINE long
mttryrecfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;
    m_atomic_t  id = (m_atomic_t)pthread_self();

    if (*lp == MTMTXINITVAL) {
        res = m_cmpswap(lp, MTMTXINITVAL, id);
    }
    if (!res) {
        res = (*lp == id);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static C_INLINE void
mtlkrecfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;
    m_atomic_t  id = (m_atomic_t)pthread_self();

    do {
        while (*lp != MTMTXINITVAL) {
            m_waitspin();
        }
        res = m_cmpswap(lp, MTMTXINITVAL, MTMTXLKVAL);
        if (!res) {
            res = (*lp == id);
        }
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
mtunlkrecfmtx(volatile m_atomic_t *lp)
{
    m_membar();         // full memory barrier
    *lp = MTMTXINITVAL;  // lazy-write
    m_endspin();        // signal wakeup-event

    return;
}

#endif /* !defined(__zen__) */

#endif /* __MT_MTX_H__ */

