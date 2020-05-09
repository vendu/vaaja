#include <stdlib.h>
#include <stdio.h>
#include <zero/cdefs.h>
#include <mach/asm.h>
#include <mt/priolk.h>

/* <vendu> eliminated the giant mutex */

THREADLOCAL struct priolkdata  *t_priolk;
volatile struct priolkdata     *g_priolkq;

void
priolkset(unsigned long prio)
{
    t_priolk->val = 1UL << prio;

    return;
}

/* initiase priority lock data for a thread */
void
priolkinit(struct priolkdata *data, unsigned long val)
{
    struct priolkdata *ptr = data;
    unsigned long      prio = 1UL << val;
    m_atomic_t         res = 0;
    struct priolkdata *head;
    struct priolkdata *next;

    if (!ptr) {
        /* structure not supplied */
        if (g_priolkq) {
            /* try to grab from free list */
            do {
                head = (struct priolkdata *)g_priolkq;
                if (head) {
                    next = (struct priolkdata *)head->next;
                    res = m_cmpswapptr((m_atomicptr_t *)&g_priolkq,
                                       (long *)head,
                                       (long *)next);
                }
            } while (!res && (g_priolkq));
            if (res) {
                /* success */
                ptr = head;
            }
        }
        if (!res) {
            /* try to allocate */
            ptr = PRIOLKALLOC(sizeof(struct priolkdata));
#if defined(PRIOLKUSEMMAP)
            if (ptr == PRIOLKALLOCFAILED) {
                ptr = NULL;
            }
#endif
        }
        if (!ptr) {
            fprintf(stderr, "PRIOLK: failed to initialise priority\n");

            exit(1);
        }
    }
    ptr->val = prio;
    ptr->orig = prio;
    t_priolk = ptr;

    return;
}

void
priolkget(struct priolk *priolk)
{
    unsigned long      prio = t_priolk->val;
    struct priolkdata *owner;
    unsigned long      mask;
    long               res;

    /* read priority atomically */
    m_membar();
    mask = prio - 1;
    while (priolk->waitbits & mask) {
        /* unfinished higher-priority tasks */
        if (t_priolk->val != prio) {
            /* priority changed, reread and update mask */
            prio = t_priolk->val;
            m_membar();
            mask = prio - 1;
        }
        m_waitspin();
    }
    /* see if no higher-priority waiters and unlocked */
    res = m_cmpswapptr((m_atomicptr_t *)&priolk->owner,
                       NULL,
                       (long *)t_priolk);
    if (res) {
        /* success */

        return;
    }
    m_atomor(&priolk->waitbits, prio);
    do {
        while (priolk->waitbits & mask) {
            if (t_priolk->val != prio) {
                /* priority changed */
                m_atomand(&priolk->waitbits, ~prio);
                prio = t_priolk->val;
                m_atomor(&priolk->waitbits, prio);
                mask = prio - 1;
            }
            m_waitspin();
        }
        /* see if no higher-priority waiters and unlocked */
        owner = m_fetchswapptr((m_atomicptr_t *)&priolk->owner,
                               NULL,
                               (long *)t_priolk);
        if (!owner) {
            /* success */
            m_atomand(&priolk->waitbits, ~prio);

            return;
        }
        if (owner->val > prio) {
            owner->val = prio;
        }
        m_waitspin();
    } while (1);

    return;
}

void
priolkrel(struct priolk *priolk)
{
    m_membar();
    priolk->owner = NULL;
    m_membar();
    t_priolk->val = t_priolk->orig;
    m_endspin();

    return;
}

void
priolkfin(void)
{
    struct priolkdata *ptr = t_priolk;
    struct priolkdata *head;

    do {
        head = (struct priolkdata *)g_priolkq;
        ptr->next = head;
        if (m_cmpswapptr((m_atomicptr_t *)&g_priolkq,
                         (long *)head,
                         (long *)ptr)) {
            t_priolk = NULL;

            break;
        }
    } while (1);

    return;
}

