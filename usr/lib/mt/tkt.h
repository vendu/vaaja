#ifndef MT_TKT_H
#define MT_TKT_H

#include <mt/conf.h>

#if defined(MT_TICKET_LOCK)

#include <endian.h>
#include <mach/param.h>
#include <env/cdefs.h>

/* REFERENCE: http://locklessinc.com/articles/locks/ */

#define MT_TKT_SPINS            16384
#define MT_TKT_SIZE             (2 * MACH_WORD_SIZE)

#if (MACH_WORD_SIZE == 4)

union mttkt {
    m_uatomic_t                 uval;
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    struct {
        uint16_t                val;
        uint16_t                cnt;
    } s;
#else
    struct {
        uint16_t                cnt;
        uint16_t                val;
    } s;
#endif
};

#elif (MACH_WORD_SIZE == 8)

union mttkt {
    m_atomicu64_t               uval;
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
    struct {
        uint32_t                val;
        uint32_t                cnt;
    } s;
#else
    struct {
        uint32_t                cnt;
        uint32_t                val;
    } s;
#endif
};

#endif /* MACH_WORD_SIZE */

#define MT_TKT_BKT_ITEMS        (MACH_CL_SIZE / MT_TKT_SIZE)
struct mttktbkt {
    union mttkt                 tab[MT_TKT_BKT_ITEMS];
};

typedef volatile union mttkt    mttkt;

#if (MACH_WORD_SIZE == 4)

/* only return when the lock appears unlocked */
static C_INLINE void
mtlktkt(mttkt *tp)
{
    uint16_t                    val = m_fetchaddu16(&tp->s.cnt, 1);

    while (tp->s.val != val) {
        m_waitspin();
    }

    return;
}

static C_INLINE void
mtunlktkt(mttkt *tp)
{
    m_membar();
    tp->s.val++;
    m_endspin();

    return;
}

/* return 1 if lock succeeds, 0 otherwise */
static C_INLINE long
mttrytkt(union mttkt *tp)
{
    uint16_t                    val = tp->s.cnt;
    uint16_t                    cnt = val + 1;
    uint32_t                    cmp = ((uint32_t)val << 16) | val;
    uint32_t                    cmpnew = ((uint32_t)cnt << 16) | val;
    long                        res = 0;

    if (m_cmpswapu32(&tp->uval, cmp, cmpnew)) {
        res++;
    }

    return res;
}

#elif (MACH_WORD_SIZE == 8)

static C_INLINE void
mtlktkt(union mttkt *tp)
{
    uint32_t                    val = m_fetchaddu32(&tp->s.cnt, 1);

    while (tp->s.val != val) {
        m_waitspin();
    }

    return;
}

static C_INLINE void
mtunlktkt(union mttkt *tp)
{
    m_membar();
    tp->s.val++;
    m_endspin();

    return;
}

/* return 1 if lock succeeds, 0 otherwise */
static C_INLINE long
mttrytkt(union mttkt *tp)
{
    uint32_t                    val = tp->s.cnt;
    uint32_t                    cnt = val + 1;
    uint64_t                    cmp = ((uint64_t)val << 32) | val;
    uint64_t                    cmpnew = ((uint64_t)cnt << 32) | val;
    long                        res = 0;

    if (m_cmpswapu64(&tp->uval, cmp, cmpnew)) {
        res++;
    }

    return res;
}

#endif /* MACH_WORD_SIZE */

#endif /* defined(MT_TICKET_LOCK) */

#endif /* MT_TKT_H */

