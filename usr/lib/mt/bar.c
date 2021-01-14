/* REFERENCE: http://locklessinc.com/articles/barriers/ */

#include <mt/bar.h>

#if (MTFMTX)
#define mtinitbarlk(lp) (*(lp) = MTFMTXINITVAL)
#define mtlkbar(lp)     mtlkfmtx(lp)
#define mtunlkbar(lp)   mtunlkfmtx(lp)
#else
#define mtinitbarlk(lp) (*(lp) = MTFMTXINITVAL)
#define mtlkbar(lp)     mtxlk(lp)
#endif

void
mtfreebar(mtbar *bar)
{
    mtlkbar(&bar->lk);
    while (bar->num > BARFLAGBIT) {
        mtwaitcond(&bar->cond, &bar->lk);
    }
    mtunlkbar(&bar->lk);
//    condfree(&bar->cond);
}

void
mtinitbar(mtbar *bar, unsigned long cnt)
{
    mtinitbarlk(&bar->lk);
    mtinitcond(&bar->cond);
    bar->cnt = cnt;
    bar->num = BARFLAGBIT;
}

long
barwait(mtbar *bar)
{
    mtlkbar(&bar->lk);
    while (bar->num > BARFLAGBIT) {
        mtwaitcond(&bar->cond, &bar->lk);
    }
    if (bar->num == BARFLAGBIT) {
        bar->num = 0;
    }
    bar->num++;
    if (bar->num == bar->cnt) {
        bar->num += BARFLAGBIT - 1;
        mtsigcondall(&bar->cond);
        mtunlkbar(&bar->lk);

        return BARSERIALTHR;
    } else {
        while (bar->num < BARFLAGBIT) {
            mtwaitcond(&bar->cond, &bar->lk);
        }
        bar->num--;
        if (bar->num == BARFLAGBIT) {
            mtsigcondall(&bar->cond);
        }
        mtunlkbar(&bar->lk);

        return 0;
    }
}

void
mtinitbarpool(mtbarpool *pool, long cnt)
{
    cnt--;
    pool->cnt.vals.seq = 0;
    pool->cnt.vals.cnt = 0;
    pool->nref = 1;
    pool->num = cnt;

    return;
}

void
mtfreebarpool(mtbarpool *pool)
{
    m_fetchadd(&pool->nref, -1);
    do {
        volatile long nref = m_atomread(&pool->nref);

        if (!nref) {

            return;
        }
        syswait(&pool->nref, nref);
    } while (1);

    return;
}

long
barwaitpool(mtbarpool *pool)
{
    long ret;

    do {
        unsigned long seq = m_atomread(&pool->cnt.vals.seq);
        long          cnt = m_fetchaddu32(&pool->cnt.vals.cnt, 1);

        if (cnt < pool->num) {
            while (m_atomread(&pool->cnt.vals.seq) == seq) {
                syswait(&pool->cnt.vals.seq, seq);
            }
            ret = 0;

            break;
        }
        if (cnt == pool->num) {
            /* mt pool->cnt.vals.cnt, increment pool->cnt.vals.seq */
            m_membar();
            pool->cnt.rst = pool->cnt.vals.seq + 1;
            m_membar();
            syswake(&pool->cnt.vals.seq);
            ret = BARSERIALTHR;

            break;
        }
        /* we were too slow, so wait for barrier to be released */
        syswait(&pool->cnt.vals.seq, seq);
    } while (1);
    if (m_fetchadd(&pool->nref, -1) == 1) {
        /* last one to wake up, wake destroying thread */
        syswait(&pool->nref, 1);
    }

    return ret;
}

