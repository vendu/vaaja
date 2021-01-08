#ifndef __SYS_ZEN_SCHED_SPS_H__
#define __SYS_ZEN_SCHED_SPS_H__

/* static priority thread scheduler for the voima game console-wannabe */

#define SPS_EV_PRIO             0
#define SPS_KBD_PRIO            1
#define SPS_MOUSE_PRIO          2
#define SPS_AUDIO_PRIO          3
#define SPS_VIDEO_PRIO          4
#define SPS_NET_PRIO            5
#define SPS_DISK_PRIO           6
#define SPS_OPT_PRIO            7
#define SPS_TAPE_PRIO           8
#define SPS_TTY_PRIO            9
#define SPS_LPR_PRIO            10
#define SPS_PRIOS               32

#define spsinittaskq(qp)        ((qp)->head = (qp)->tail = (qp)->dummy)
struct spstaskqueue {
    m_atomic_t  dummy;      // in empty queue, both head and tail point here
    m_atomic_t  nref;       // reference count for queue manipulation
    void       *head;       // head pointer (or dummy for empty queue)
    void       *tail;       // tail pointer (or dummy for empty queue)
};

/*
 * single item queue is represented so like
 * list {
 *     .head = &list.dummy;
 *     .tail = &list.dummy;
 * };
 */
static struct spstaskqueue      spstaskq[SPS_PRIOS];

static void
spsqueuetask(struct spstaskqueue *queue, struct task *task)
{
    m_atomic_t      n;
    m_atomic_t     *ptr = &queue->dummy;
    uintptr_t       hptr;
    uintptr_t       tptr;

    if (task) {
        mtxlkbit(&queue->head, MT_ADR_LK_BIT_OFS);
        /* lock queue head */
        hptr = (uintptr_t)queue->head & ~MT_ADR_LK_BIT;
        if (hptr) {
            if (hptr == (uintptr_t)ptr) {
                /*
                 * 0 items -> 1 item
                 * -------
                 * queue.head = &queue->dummy;
                 * queue.tail = &queue->dummy;
                 */
                /* lock queue->tail */
                mtxlkbit((m_atomic_t *)&queue->tail, MT_ADR_LK_BIT_OFS);
                tptr = (uintptr_t)queue->tail & ~MT_ADR_LK_BIT;
                if (tptr == (uintptr_t)ptr) {
                    task->prev = NULL;
                    task->next = NULL;
                    /* queue tail == queue->dummy, append task to tail */
                    m_atomwrite((m_atomic_t)&queue->tail, task);
                    /* unlock/set queue->head to task */
                    m_atomwrite((m_atomic_t)&queue->head, task);
                }
            } else {
                /* queue->head != queue->dummy, lock queue->tail */
                mtxlkbit((m_atomic_t *)&queue->tail, MT_ADR_LK_BIT_OFS);
                tptr = (uintptr_t)queue->tail & ~MT_ADR_LK_BIT;
                if (tptr != hptr) {
                    /*
                     * > 2 items -> > 3 items
                     * ---------
                     * queue.head = task1;
                     * queue.tail = task2;
                     */
                    /* queue->tail != queue->head, unlock queue->head */
                    /* unlock/set queue->tail to task */
                    task->prev = (struct task *)tptr;
                    (struct task *)tptr->next = task;
                } else {
                    /*
                     * 1 item -> 2 items
                     * ------
                     * queue.head = task;
                     * queue.tail = task;
                     */
                    /* tptr == hptr */
                    /* queue->tail == queue->head, unlock queue->head */
                    task->prev = (struct task *)tptr;
                }
                m_atomwrite((m_atomic_t *)&queue->tail, task);
                /* unlock queue->head */
                m_atomwrite((m_atomic_t *)&queue->head, hptr);
            }
        }
    }

    return;
}

static struct task *
spsdequetask(struct spstaskqueue *queue, struct task *task)
{
    struct task    *qtask;
    m_atomic_t      n;
    m_atomic_t     *ptr = &queue->dummy;
    uintptr_t       hptr;
    uintptr_t       tptr;

    if (task) {
        if (queue->head) {
            mtxlkbit(&queue->head, MT_ADR_LK_BIT_OFS);
            /* lock queue head */
            hptr = (uintptr_t)queue->head & ~MT_ADR_LK_BIT;
            if (hptr != (uintptr_t)ptr) {
                /* queue->head != queue->dummy, lock queue->tail */
                mtxlkbit((m_atomic_t *)&queue->tail, MT_ADR_LK_BIT_OFS);
                tptr = (uintptr_t)queue->tail & ~MT_ADR_LK_BIT;
                if (tptr != hptr) {
                    /*
                     * >= 2 items
                     * ----------
                     * queue.head = task1;
                     * queue.tail = task2;
                     */
                    /* queue->tail != queue->head, unlock queue->head */
                    /* unlock/set queue->tail to task */
                    qtask = (struct task *)tptr;
                    tptr = (struct)tptr->prev;
                    m_atomwrite((m_atomic_t *)&queue->tail, tptr);
                    /* unlock queue->head */
                    m_atomwrite((m_atomic_t *)&queue->head, hptr);
                } else {
                    /*
                     * 1 item
                     * ------
                     * queue.head = task;
                     * queue.tail = task;
                     */
                    /* tptr == hptr */
                    /* queue->tail == queue->head, unlock queue->head */
                    qtask = (struct task *)tptr;
                    m_atomwrite((m_atomic_t *)&queue->tail, ptr);
                    m_atomwrite((m_atomic_t *)queue->head, ptr);
                }
            }
        }
    }

    return qtask;
}

struct sps {
    struct task    *taskqueue[SPS_PRIOS];
};

#endif /* __SYS_ZEN_SCHED_SPS_H__ */

