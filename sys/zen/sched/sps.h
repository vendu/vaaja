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

struct spstaskqueue {
    m_atomic_t  nref;       // reference count for queue manipulation
    m_atomic_t  dummy;      // in empty queue, both head and tail point here
    void       *head;
    void       *tail;
};

static struct   spstaskqueue[SPS_PRIOS];

static void
spsqueuetask(struct spstaskqueue *queue, struct task *task)
{
    m_atomic_t      n;
    m_atomic_t     *ptr = &queue->dummy;
    uintptr_t       uptr;

    if (task) {
        mtxlkbit(&queue->head, MT_ADR_LK_BIT_OFS);
        uptr = (uintptr_t)queue->head & ~MT_ADR_LK_BIT;
        if (uptr == (uintptr_t)ptr) {
            mtxlkbit((m_atomic_t *)&queue->tail, MT_ADR_LK_BIT_OFS);
            uptr = (uintptr_t)queue->tail & ~MT_ADR_LK_BIT;
            if (uptr == (uintptr_t)ptr) {
                m_atomwrite((m_atomic_t)&queue->tail, task);
            }
            m_atomwrite((m_atomic_t)&queue->head, task);
        } else {
            mtxlkbit((m_atomic_t *)&queue->tail, MT_ADR_LK_BIT_OFS);
            uptr = (uintptr_t)queue->tail & ~MT_ADR_LK_BIT;
            if (uptr == (uintptr_t)ptr) {
                uptr = (uintptr_t)queue->head & ~MT_ADR_LK_BIT;
                task->prev = (void *)uptr;
                if (uptr) {
                    (struct task *)uptr->next = task;
                }
            }
            mtxunlkbit((m_atomic_t *)&queue->tail, MT_AD_LK_BIT_OFS);
            mtxunlkbit((m_atomic_t *)&queue->head, MT_AD_LK_BIT_OFS);
        }
    }

    return;
}

struct sps {
    struct task    *taskqueue[SPS_PRIOS];
};

#endif /* __SYS_ZEN_SCHED_SPS_H__ */

