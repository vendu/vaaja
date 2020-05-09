#include <mach/asm.h>
#include <zero/lwlk.h>
#include <mt/waitq.h>

/* FIXME: the stuff here is broken :) */

static long
lwlklkasync(struct lwlk *lk, long wrlk)
{
    struct waitq *wq;
    struct lwlk  *oldlk;
    struct lwlk  *newlk;

    wq = waitqget();
    do {
        oldlk = lk;
        newlk = lk;
        if (!wrlk && !(oldlk->flg & LWLK_WRLK_BIT)
            && (oldlk->waitq == WAITQ_NONE
                || (oldlk->flg & LWLK_RDBIAS_BIT))) {
            /* read lock */
            oldlk->nread++;
        } else if ((wrlk) &&
                   !((newlk->flg & LWLK_WRLK_BIT) || (newlk->nread > 0))) {
            /* write lock */
            newlk->flg |= LWLK_WRLK_BIT;
        } else {
            /* need to block */
            wq->status = LWLK_WRLK;
            wq->next = oldlk->waitq;
            newlk->waitq = (uintptr_t)wq;
        }
    } while (!m_cmpswapptr((m_atomicptr_t *)lk,
                           (m_atomicptr_t *)oldlk,
                           (m_atomicptr_t *)newlk));
    if (newlk->waitq == wq->id) {

        return LWLK_FAILURE;
    }

    return LWLK_SUCCESS;
}

static void
lwlklk(struct lwlk *lk, long wrlk)
{
    struct waitq *wq = waitqget();

    if (!lwlklkasync(lk, wrlk)) {
        wq->wait();
    }

    return;
}

static void
lwlkunlkfair(struct lwlk *lk)
{
    struct waitq *wq;
    struct lwlk  *oldlk;
    struct lwlk  *newlk;

    do {
        oldlk = lk;
        newlk = lk;
        if (newlk->flg & LWLK_WRLK_BIT) {
            newlk->flg &= ~LWLK_WRLK_BIT;
        } else {
            newlk->nread--;
        }
        if (!((newlk->flg & LWLK_WRLK_BIT) || (newlk->nread > 0))) {
            wq = waitqgetnext(newlk->waitq);
            newlk->waitq = (uintptr_t)wq;
            if (wq != NULL) {
                wq = waitqfind(wq);
                if (wq->status != LWLK_WRLK) {
                    newlk->nread = waitqgetsize(wq);
                } else {
                    newlk->flg |= LWLK_WRLK_BIT;
                }
            }
        }
    } while (!m_cmpswapptr((m_atomicptr_t *)lk,
                           (m_atomicptr_t *)oldlk,
                           (m_atomicptr_t *)newlk));
    if (wq) {
        wq->next = WAITQ_NONE;
    }
    wq->signal();

    return;
}

