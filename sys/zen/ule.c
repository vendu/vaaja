#include <zen/ule.h>

extern struct proc     *k_proczombietab[TASKSMAX];

#include <kern/nice.h>

struct schednice       *k_schedniceptr = &k_schednicetab[SCHEDNICEHALF];
static struct tasktab   k_scheddeadlinetab[SCHEDNLVL0DL];
static struct task     *k_schedstoppedtab[TASKSMAX];
struct task            *k_schedreadytab0[SCHEDNQUEUE];
struct task            *k_schedreadytab1[SCHEDNQUEUE];
static struct task     *k_schedidletab[SCHEDNIDLE];
static long             k_schedreadymap0[SCHEDREADYMAPNWORD];
static long             k_schedreadymap1[SCHEDREADYMAPNWORD];
static long             k_schedidlemap[SCHEDIDLEMAPNWORD];
/* SCHEDIDLE queues are not included in SCHEDNQUEUE */
static long             k_schedloadmap[SCHEDLOADMAPNWORD];
static long             k_scheddeadlinemap[SCHEDDEADLINEMAPNWORD];
long                    k_schedidlecoremap[SCHEDIDLECOREMAPNWORD];
struct schedqueueset    k_schedreadyset;

void
schedinit(void)
{
#if 0
#if (ZEROSCHED)
    schedswitch = k_schedswitchtask;
#else
#error define supported scheduler such as ZEROSCHED
#endif
#endif
    kprintf("SCHEDHISTORYSIZE == %d\n", SCHEDHISTORYSIZE);
    fastu16divu16gentab(k_fastu16divu16tab, SCHEDDIVU16TABSIZE);
    schedinitset();

    return;
}

#if (ZEROSCHED)

#define __LONGBITS     (CHAR_BIT * LONGSIZE)
#if (__LONGBITS == 32)
#define __LONGBITSLOG2 5
#elif (__LONGBITS == 64)
#define __LONGBITSLOG2 6
#endif

void
schedinitset(void)
{
    long                  lim = CPUSMAX;
    struct schedqueueset *set = &k_schedreadyset;
    //    long                  unit = k_curcpu->unit;
    long                  id;

    lim >>= __LONGBITSLOG2;
    for (id = 0 ; id < lim ; id++) {
        k_schedidlecoremap[id] = ~1L;
    }
    //    clrbit(k_schedidlecoremap, unit);
    set->curmap = k_schedreadymap0;
    set->nextmap = k_schedreadymap1;
    set->idlemap = k_schedidlemap;
    set->loadmap = k_schedloadmap;
    set->cur = k_schedreadytab0;
    set->next = k_schedreadytab1;
    set->idle = k_schedidletab;

    return;
}

#define DEQ_SINGLE_TYPE
#define DEQ_TYPE struct task
#include <zero/deq.h>

void
schedsetdeadline(struct task *task)
{
    struct tasktab   *l0tab;
    struct tasklist  *list;
    time_t            deadline = task->timelim;
    unsigned long     key0 = taskdlkey0(deadline);
    unsigned long     key1 = taskdlkey1(deadline);
    unsigned long     key2 = taskdlkey2(deadline);
    long             *map = k_scheddeadlinemap;
    void             *ptr = NULL;
    void            **pptr = NULL;
    long              fail = 0;
    void             *ptab[SCHEDNDLKEY - 1] = { NULL, NULL };

    tktlk(&k_scheddeadlinetab[key0].lk);
    l0tab = &k_scheddeadlinetab[key0];
    ptr = l0tab->tab;
    pptr = ptr;
    if (!ptr) {
        ptr = kmalloc(SCHEDNLVL1DL * sizeof(struct tasklist));
        if (ptr) {
            kbzero(ptr, SCHEDNLVL1DL * sizeof(struct tasklist));
        }
        l0tab->tab = ptr;
        ptab[0] = ptr;
        pptr = ptr;
    }
    if (ptr) {
        ptr = pptr[key1];
        if (!ptr) {
            list = kmalloc(SCHEDNLVL2DL * sizeof(struct tasklist));
            if (list) {
                kbzero(list, SCHEDNLVL2DL * sizeof(struct tasklist));
            }
            ptab[1] = list;
            pptr[key1] = list;
        } else {
            list = pptr[key1];
        }
    } else {
        fail = 1;
    }
    if (!fail) {
        list = &list[key2];
        deqappend(task, &list->ptr);
        list = ptab[0];
        list->ptr = ptab[1];
        list = ptab[1];
    }
    tktunlk(&k_scheddeadlinetab[key0].lk);

    return;
}

void
schedsetready(struct task *task)
{
    long                   sched = task->sched;
    long                   prio = task->prio;
    struct schedqueueset  *set = &k_schedreadyset;
    struct task          **queue;
    long                  *map;
    long                   score;
    long                   type;
    long                   ndx;
    long                   qid;
    long                   load;
    long                   lim;
    long                   flg;

    type = sched;
    if (sched == SCHEDDEADLINE) {
        /* SCHEDDEADLINE */
        schedsetdeadline(task);

        return;
    } else if (sched <= SCHEDSYSTEM) {
        /* SCHEDTRAP, SCHEDREALTIME or SCHEDSYSTEM */
        if (sched == SCHEDREALTIMERR) {
            type = SCHEDREALTIME;
        }
        if (type == SCHEDTRAP) {
            qid = schedcalctrapqueue(prio);
        } else if (type == SCHEDREALTIME) {
            qid = schedcalcqueue(type, prio);
        } else {
            /* SCHEDSYSTEM */
            lim = schedclassmaxprio(SCHEDSYSTEM);
            if (prio < lim) {
                prio++;
                task->prio = prio;
            }
            qid = schedcalcqueue(SCHEDSYSTEM, prio);
        }
        tktlk(&set->lk);
        map = set->curmap;
        queue = &set->cur[qid];
        setbit(map, qid);
    } else if (sched < SCHEDIDLE) {
        /* SCHEDNORMAL, SCHEDBATCH */
        flg = task->flg;
        score = 0;
        type = sched;
        if (task->sched == SCHEDRESPONSIVE) {
            lim = schedclassmaxprio(SCHEDRESPONSIVE);
            prio++;
            if (prio == lim) {
                type = SCHEDNORMAL;
            }
        } else if (flg & TASKHASINPUT) {
            /* boost user-interrupt task to highest priority */
            sched = SCHEDRESPONSIVE;
            prio = schedclassminprio(SCHEDRESPONSIVE);
        } else {
            /* SCHEDNORMAL or SCHEDBATCH; calculate timeshare priority */
            prio = schedcalcintparm(task, &score);
        }
        task->sched = type;
        task->prio = prio;
        qid = schedcalcqueue(type, prio);
        if (schedisinteract(score)) {
            /* if interactive, insert onto current queue */
            tktlk(&set->lk);
            map = set->curmap;
            queue = &set->cur[qid];
        } else {
            /* if not interactive, insert onto next queue */
            tktlk(&set->lk);
            map = set->nextmap;
            queue = &set->next[qid];
        }
        setbit(map, qid);
    } else {
        /* SCHEDIDLE */
        lim = SCHEDNCLASS * SCHEDNCLASSPRIO + SCHEDNIDLE - 1;
        if (prio < lim) {
            prio++;
            task->prio = prio;
        }
        qid = schedcalcqueue(SCHEDIDLE, prio);
        ndx = qid - schedclassminqueue(SCHEDIDLE);
        tktlk(&set->lk);
        map = set->idlemap;
        queue = &set->idle[ndx];
        setbit(map, ndx);
    }
    load = set->loadmap[qid];
    load++;
    if (sched != SCHEDREALTIMERR) {
        deqappend(task, queue);
    } else {
        /* SCHEDREALTIME with SCHED_FIFO */
        deqpush(task, queue);
    }
    set->loadmap[qid] = load;
    tktunlk(&set->lk);

    return;
}

void
schedsetstopped(struct task *task)
{
    long id = task->id;

    k_schedstoppedtab[id] = task;

    return;
}

void
schedsetzombie(struct proc *proc)
{
    long qid = proc->ppid;      // FIXME: might need to use proc->pgrp here

    k_proczombietab[qid] = proc;

    return;
}

/* switch tasks */
//schedswitchtask(struct task *curtask)
FASTCALL NORETURN void
schedswitchtask(struct task *curtask)
{
    //    long                   unit = k_curcpu->unit;
    //    struct task           *curtask = (struct task *)k_curtask;
    long                   unit = curtask->unit;
    long                   state = (curtask) ? curtask->state : -1;
    struct schedqueueset  *set = &k_schedreadyset;
    struct task           *task;
    struct task           *next;
    struct task          **queue;
    long                  *map;
    long                   val;
    long                   ndx;
    long                   ofs;
    long                   lim;
    long                   loop;

    if (!curtask) {
        /* kernel initialisations are still taking place */

        m_taskjmp(&curtask->m_task);
    }
    switch (state) {
        case TASKNEW:
        case TASKREADY:
            schedsetready(curtask);

            break;
        case TASKSLEEPING:
            schedsetsleep(curtask);

            break;
        case TASKSTOPPED:
            schedsetstopped(curtask);

            break;
        case TASKZOMBIE:
            schedsetzombie(curtask->proc);

            break;
        default:
            kpanic(-1, 0, NULL); /* FIXME: error # */

            break;
    }
    do {
        loop = 1;
        do {
            /* loop over current and next priority-queues */
            lim = SCHEDNQUEUE;
            lim >>= __LONGBITSLOG2;
            tktlk(&set->lk);
            map = set->curmap;
            queue = &set->cur[SCHEDTRAPPRIOMIN];
            for (ndx = 0 ; ndx < lim ; ndx++) {
                val = map[ndx];
                if (val) {
                    ofs = ndx * CHAR_BIT * sizeof(long);
                    ndx = tzerol(val);
                    ofs += ndx;
                    if (ofs < SCHEDNQUEUE) {
                        queue += ofs;
                        task = deqpop(queue);
                        if (!queue) {
                            m_clrbit((m_atomic_t *)map, ofs);
                        }
                        tktunlk(&set->lk);

                        m_taskjmp(&task->m_task);
                    }
                }
            }
            if (loop) {
                /* if no task found during the first iteration, switch queues */
                schedswapqueues();
            }
            tktunlk(&set->lk);
        } while (loop--);
        /* if both current and next queues are empty, look for an idle task */
        lim = SCHEDNIDLE;
        lim >>= __LONGBITSLOG2;
        tktlk(&set->lk);
        map = &set->idlemap[0];
        queue = set->idle;
        for (ndx = 0 ; ndx < lim ; ndx++) {
            val = map[ndx];
            if (val) {
                ofs = ndx * CHAR_BIT * sizeof(long);
                ndx = tzerol(val);
                ofs += ndx;
                if (ofs < SCHEDNIDLE) {
                    queue += ofs;
                    task = deqpop(queue);
                    if (!queue) {
                        m_clrbit((m_atomic_t *)map, ofs);
                    }
                    tktunlk(&set->lk);

                    m_taskjmp(&task->m_task);
                }
            }
        }
        /* FIXME: try to pull threads from other cores here */
//        task = taskpull(unit);
        /* mark the core as idle */
        map = &set->idlemap[0];
        setbit(map, unit);
        tktunlk(&set->lk);
        k_intron();
        m_waitint();
    } while (1);
    m_taskjmp(&task->m_task);
}

#if 0
void
schedyield(void)
{
    struct task *curtask = k_getcurtask();

    schedswitchtask(curtask);
}
#endif

NORETURN void
schedyield(void)
{
    struct task *oldtask = k_getcurtask();
    //    struct task *task = NULL;

    schedswitchtask(oldtask);
    //    task = schedswitchtask(oldtask);
    //    //    if (task != oldtask) {
    //    m_taskjmp(&task->m_task);
    //    }
}

#endif /* ZEROSCHED */

