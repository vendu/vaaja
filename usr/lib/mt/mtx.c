/* REFERENCE: http://preshing.com/20120305/implementing-a-recursive-mutex/ */

#include <mt/conf.h>

#if defined(MT_MUTEX) && !defined(MT_POSIX_THREAD)

#include <stdlib.h>
#include <mt/mtx.h>

zenmtxatr *
zenallocmtxatr(void)
{
    zenmtxatr *atr = malloc(sizeof(zenmtxatr));

    if (atr) {
        atr->flg = __MT_MTXATR_DYNAMIC;
    }

    return atr;
}

int
zenfreemtxatr(zenmtxatr *atr)
{
    int ret = 0;

    if (atr->flg & __MT_MTXATR_DYNAMIC) {
        free(atr);
    } else {
        ret = MT_MTXATR_NOTDYNAMIC;
    }

    return ret;
}

int
zeninitmtxatr(zenmtxatr *atr)
{
    long dynflg = atr->flg & __MT_MTXATR_DYNAMIC;

    if (!atr) {

        return -1;
    }
    atr->flg |= __MT_MTXATR_INIT | dynflg;

    return 0;
}

long
zentrylkmtx(zenmtx *mtx)
{
    volatile long res = 0;
    long          thr;

    if (!(mtx->atr.flg & MT_MUTEX_RECURSIVE)) {
        /* non-recursive mutex */
        res = mtxtrylk(&mtx->val);
    } else {
        /* recursive mutex */
        thr = thrid();
        if (mtx->val == thr) {
            m_atominc(&mtx->cnt);
        } else if (!m_cmpswap(&mtx->cnt, 0, 1)) {
            mtx->val = thr;
        } else {

            return 0;
        }
        res = thr;
        mtx->rec++;
    }

    return res;
}

void
zenlkmtx(zenmtx *mtx)
{
    volatile long res = -1;
    long          thr;

    if (!(mtx->atr.flg & MT_MUTEX_RECURSIVE)) {
        /* non-recursive mutex */
        mtxlk(&mtx->val);
    } else {
        /* recursive mutex */
        thr = thrid();
        res = m_fetchadd(&mtx->cnt, 1);
        if (res) {
            if (mtx->val != thr) {
                do {
                    res = mtxtrylk(&mtx->lk);
                    if (!res) {
                        thryield();
                    }
                } while (!res);
            }
        }
        mtx->val = thr;
        mtx->rec++;
    }

    return;
}

void
zenunlkmtx(zenmtx *mtx)
{
    volatile long res;
    long          thr;

    if (!(mtx->atr.flg & MT_MUTEX_RECURSIVE)) {
        /* non-recursive mutex */
        mtxunlk(&mtx->val);
    } else {
        /* recursive mutex */
        thr = thrid();
        if (mtx->val == thr) {
            mtx->rec--;
            if (!mtx->rec) {
                mtx->val = MT_MUTEX_FREE;
            }
            res = m_fetchadd(&mtx->cnt, -1);
            if (res > 1) {
                if (!mtx->rec) {
                    mtxunlk(&mtx->lk);
                }
            }
        }
    }

    return;
}

#endif /* defined(MT_MUTEX) && !defined(MT_POSIX_THREAD) */

