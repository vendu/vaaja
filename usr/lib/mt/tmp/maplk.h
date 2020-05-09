#ifndef __MT_MAPLK_H__
#define __MT_MAPLK_H__

#include <mach/asm.h>
#include <limits.h>
#include <mach/param.h>

typedef struct {
    unsigned long  nbit;
    volatile long *bits;
} zeromaplk;

zeromaplk * maplkinit(zeromaplk *maplk, unsigned long n);

static __inline__ long
maplktrybit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);
    long val;
    long ret;

    val = m_cmpsetbit(&maplk->bits[word], id);
    ret = !val;

    return ret;
}

static __inline__ void
maplkbit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);
    long val;

    do {
        val = m_cmpsetbit(&maplk->bits[word], id);
    } while (val);

    return;
}

static __inline__ void
mapunlkbit(zeromaplk *maplk, long ndx)
{
    long word = ndx >> LONGSIZELOG2;
    long id = word & (LONGSIZE * CHAR_BIT - 1);

    m_cmpclrbit(&maplk->bits[word], id);

    return;
}

#endif /* __MT_MAPLK_H__ */

