#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/atomic.h>
#include <mach/asm.h>

#define V0_MEM_LK_BIT_32    (INT32_C(1) << V0_MEM_LK_BIT_OFS)
#define V0_MEM_LK_BIT_OFS   1
#define V0_MTX_INIT_VAL_32  INT32_C(0)
#define V0_MTX_LOCK_VAL_32  INT32_C(1)

static C_INLINE int
v0trylkbit32(volatile m_atomic32_t *lp, long pos)
{
    m_atomic32_t    res = 0;

    res = !m_cmpsetbit32(lp, pos);

    return (int)res;
}

static C_INLINE int
v0lkbit32(volatile m_atomic32_t *lp, long pos)
{
    m_atomic32_t    res = 0;
    m_atomic32_t    bit = V0_MEM_LK_BIT_32;

    do {
        while (*lp & bit) {
            m_waitspin();
        }
        res = !m_cmpsetbit32(lp, pos);
        if (!res) {
            m_waitspin();
        }
    } while (!res);

    return (int)res;
}

/*
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
v0unlkbit32(volatile m_atomic32_t *lp, long pos)
{
    m_membar();             // full memory barrier
    m_clrbit32(lp, pos);    // clear bit
    m_endspin();            // signal wakeup-event

    return;
}

/*
 * try to acquire fast mutex lock
 * - return non-zero on success, zero if already locked
 */
static C_INLINE long
v0tryfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;

    if (*lp == V0_MTX_INIT_VAL_32) {
        res = m_cmpswap(lp, V0_MTX_INIT_VAL_32, V0_MTX_LOCK_VAL_32);
    }

    return res;
}

/*
 * - acquire fast mutex lock
 * - spin on volatile lock to avoid excess lock-operations
 */
static C_INLINE void
v0lkfmtx(volatile m_atomic_t *lp)
{
    m_atomic_t  res = 0;

    do {
        while (*lp != V0_MTX_INIT_VAL_32) {
            m_waitspin();
        }
        res = m_cmpswap(lp, V0_MTX_INIT_VAL_32, V0_MTX_LOCK_VAL_32);
    } while (!res);

    return;
}

/*
 * unlock fast mutex
 * - must use full memory barrier to guarantee proper write-ordering
 */
static C_INLINE void
v0unlkfmtx(volatile m_atomic_t *lp)
{
    m_membar();                 // full memory barrier
    *lp = V0_MTX_INIT_VAL_32;   // lazy-write
    m_endspin();                // signal wakeup-event

    return;
}

