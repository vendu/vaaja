#include <limits.h>
#include <zero/cdefs.h>
#include <zen/util.h>
#include <zen/fastudiv.h>
#include <zen/sched/ule.h>
#include <zen/nice.h>
#include <mt/tktlk.h>

#if defined(ZEN_ULE_TASK_SCHED)

#define kgethz()        ZEN_ULE_HZ

#define __LONGBITS     (CHAR_BIT * MACH_WORD_SIZE)
#if (__LONGBITS == 32)
#define __LONGBITSLOG2 5
#elif (__LONGBITS == 64)
#define __LONGBITSLOG2 6
#endif

extern struct zenproc  *k_proczombietab[MACH_MAX_PROCS];

struct divu16           k_fastu16divu16tab[ULE_DIVU16TAB_SIZE];

struct zentask          k_scheddeadlinetab[ULE_TAB_QUEUES];
struct zentask         *k_schedstoppedtab[MACH_MAX_THREADS];
struct zentask         *k_schedreadytab0[ULE_CLASS_QUEUES];
struct zentask         *k_schedreadytab1[ULE_CLASS_QUEUES];
struct zentask         *k_schedidletab[ULE_CLASS_QUEUES];
long                    k_schedreadymap0[ULE_CLASS_MAP_WORDS];
long                    k_schedreadymap1[ULE_CLASS_MAP_WORDS];
long                    k_schedidlemap[ULE_CLASS_MAP_WORDS];
/* ULE_IDLE queues are not included in ULE_CLASS_QUEUES */
long                    k_schedloadmap[ULE_CLASS_MAP_WORDS];
long                    k_scheddeadlinemap[ULE_CLASS_MAP_WORDS];
long                    k_schedidlecoremap[ULE_CLASS_MAP_WORDS];
struct schedqueueset    k_schedreadyset;
struct schednice       *k_schedniceptr = &k_schednicetab[ULE_NICE_HALF];

void
schedinitset(void)
{
    long                  lim = ZEN_MAX_CPUS;
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
    kprintf("ULE_HISTORY_SIZE == %d\n", ULE_HISTORY_SIZE);
    fastu16divu16init(k_fastu16divu16tab, ULE_DIVU16TAB_SIZE);
    schedinitset();

    return;
}

#define DEQ_SINGLE_TYPE
#define DEQ_TYPE struct zentask
#include <zero/deq.h>

void
schedsetdeadline(struct zentask *task)
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
    void             *ptab[ULE_NDLKEY - 1] = { NULL, NULL };

    mtlktkt(&k_scheddeadlinetab[key0].lk);
    l0tab = &k_scheddeadlinetab[key0];
    ptr = l0tab->tab;
    pptr = ptr;
    if (!ptr) {
        ptr = kmalloc(ULE_NLVL1DL * sizeof(struct tasklist));
        if (ptr) {
            kbzero(ptr, ULE_NLVL1DL * sizeof(struct tasklist));
        }
        l0tab->tab = ptr;
        ptab[0] = ptr;
        pptr = ptr;
    }
    if (ptr) {
        ptr = pptr[key1];
        if (!ptr) {
            list = kmalloc(ULE_NLVL2DL * sizeof(struct tasklist));
            if (list) {
                kbzero(list, ULE_NLVL2DL * sizeof(struct tasklist));
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
    mtunlktkt(&k_scheddeadlinetab[key0].lk);

    return;
}

void
schedsetready(struct zentask *task)
{
    long                   sched = task->sched;
    long                   prio = task->prio;
    struct schedqueueset  *set = &k_schedreadyset;
    struct zentask       **queue;
    long                  *map;
    long                   score;
    long                   type;
    long                   ndx;
    long                   qid;
    long                   load;
    long                   lim;
    long                   flg;

    type = sched;
    if (sched == ULE_DEADLINE) {
        /* ULE_DEADLINE */
        schedsetdeadline(task);

        return;
    } else if (sched <= ULE_SYSTEM) {
        /* ULE_TRAP, ULE_REALTIME or ULE_SYSTEM */
        if (sched == ULE_REALTIMERR) {
            type = ULE_REALTIME;
        }
        if (type == ULE_TRAP) {
            qid = schedcalctrapqueue(prio);
        } else if (type == ULE_REALTIME) {
            qid = schedcalcqueue(type, prio);
        } else {
            /* ULE_SYSTEM */
            lim = schedclassmaxprio(ULE_SYSTEM);
            if (prio < lim) {
                prio++;
                task->prio = prio;
            }
            qid = schedcalcqueue(ULE_SYSTEM, prio);
        }
        mtlktkt(&set->lk);
        map = set->curmap;
        queue = &map[qid];
        setbit(map, qid);
    } else if (sched < ULE_IDLE) {
        /* ULE_NORMAL, ULE_BATCH */
        flg = task->flg;
        score = 0;
        type = sched;
        if (task->sched == ULE_RESPONSIVE) {
            lim = schedclassmaxprio(ULE_RESPONSIVE);
            prio++;
            if (prio == lim) {
                type = ULE_NORMAL;
            }
        } else if (flg & ULE_TASK_HAS_INPUT) {
            /* boost user-interrupt task to highest priority */
            sched = ULE_RESPONSIVE;
            prio = schedclassminprio(ULE_RESPONSIVE);
        } else {
            /* ULE_NORMAL or ULE_BATCH; calculate timeshare priority */
            prio = schedcalcintparm(task, &score);
        }
        task->sched = type;
        task->prio = prio;
        qid = schedcalcqueue(type, prio);
        if (schedisinteract(score)) {
            /* if interactive, insert onto current queue */
            mtlktkt(&set->lk);
            map = set->curmap;
            queue = &map[qid];
        } else {
            /* if not interactive, insert onto next queue */
            mtlktkt(&set->lk);
            map = set->nextmap;
            queue = &map[qid];
        }
        setbit(map, qid);
    } else {
        /* ULE_IDLE */
        lim = ULE_CLASSES * ULE_CLASS_PRIOS + ULE_IDLE_QUEUES - 1;
        if (prio < lim) {
            prio++;
            task->prio = prio;
        }
        qid = schedcalcqueue(ULE_IDLE, prio);
        ndx = qid - schedclassminqueue(ULE_IDLE);
        mtlktkt(&set->lk);
        map = set->idlemap;
        queue = &map[ndx];
        setbit(map, ndx);
    }
    load = set->loadmap[qid];
    load++;
    if (sched != ULE_REALTIMERR) {
        deqappend(task, queue);
    } else {
        /* ULE_REALTIME with ULE__FIFO */
        deqpush(task, queue);
    }
    set->loadmap[qid] = load;
    mtunlktkt(&set->lk);

    return;
}

void
schedsetstopped(struct zentask *task)
{
    long id = task->id;

    k_schedstoppedtab[id] = task;

    return;
}

void
schedsetzombie(struct zenproc *proc)
{
    long qid = proc->ppid;      // FIXME: might need to use proc->pgrp here

    k_proczombietab[qid] = proc;

    return;
}

/* switch tasks */
//schedswitchtask(struct zentask *curtask)
C_FASTCALL C_NORETURN void
schedswitchtask(struct zentask *curtask)
{
    //    long                      Unit = k_curcpu->unit;
    //    struct zentask           *curtask = (struct zentask *)k_curtask;
    long                   unit = curtask->unit;
    long                   state = (curtask) ? curtask->state : -1;
    struct schedqueueset  *set = &k_schedreadyset;
    struct zentask        *task;
    struct zentask        *next;
    struct zentask       **queue;
    long                  *map;
    long                   val;
    long                   ndx;
    long                   ofs;
    long                   lim;
    long                   loop;

    if (!curtask) {
        /* kernel initialisations are still taking place */

        m_taskjmp(&curtask->m_tcb);
    }
    switch (state) {
        case ZEN_PROC_NEW:
        case ZEN_PROC_READY:
            schedsetready(curtask);

            break;
        case ZEN_PROC_SLEEP:
            schedsetsleep(curtask);

            break;
        case ZEN_PROC_STOPPED:
            schedsetstopped(curtask);

            break;
        case ZEN_PROC_ZOMBIE:
            schedsetzombie(&curtask->proc);

            break;
        default:
            kpanic(-1, 0, NULL); /* FIXME: error # */

            break;
    }
    do {
        loop = 1;
        do {
            /* loop over current and next priority-queues */
            lim = ULE_CLASS_QUEUES;
            lim >>= __LONGBITSLOG2;
            mtlktkt(&set->lk);
            map = set->curmap;
            queue = &map[ULE_TRAP_PRIO_MIN];
            for (ndx = 0 ; ndx < lim ; ndx++) {
                val = map[ndx];
                if (val) {
                    ofs = ndx * CHAR_BIT * sizeof(long);
                    ndx = tzerol(val);
                    ofs += ndx;
                    if (ofs < ULE_CLASS_QUEUES) {
                        queue += ofs;
                        task = deqpop(queue);
                        if (!queue) {
                            m_clrbit((m_atomic_t *)map, ofs);
                        }
                        mtunlktkt(&set->lk);

                        m_taskjmp(&task->m_tcb);
                    }
                }
            }
            if (loop) {
                /* if no task found during the first iteration, switch queues */
                schedswapqueues();
            }
            mtunlktkt(&set->lk);
        } while (loop--);
        /* if both current and next queues are empty, look for an idle task */
        lim = ULE_IDLE_QUEUES;
        lim >>= __LONGBITSLOG2;
        mtlktkt(&set->lk);
        map = set->idlemap;
        queue = map;
        for (ndx = 0 ; ndx < lim ; ndx++) {
            val = map[ndx];
            if (val) {
                ofs = ndx * CHAR_BIT * sizeof(long);
                ndx = tzerol(val);
                ofs += ndx;
                if (ofs < ULE_IDLE_QUEUES) {
                    queue += ofs;
                    task = deqpop(queue);
                    if (!queue) {
                        m_clrbit((m_atomic_t *)map, ofs);
                    }
                    mtunlktkt(&set->lk);

                    m_taskjmp(&task->m_tcb);
                }
            }
        }
        /* FIXME: try to pull threads from other cores here */
//        task = taskpull(unit);
        /* mark the core as idle */
        map = set->idlemap;
        setbit(map, unit);
        mtunlktkt(&set->lk);
        k_intron();
        m_waitint();
    } while (1);
    m_taskjmp(&task->m_tcb);
}

#if 0
void
schedyield(void)
{
    struct zentask *curtask = k_getcurtask();

    schedswitchtask(curtask);
}
#endif

NORETURN void
schedyield(void)
{
    struct zentask *oldtask = k_getcurtask();
    //    struct zentask *task = NULL;

    schedswitchtask(oldtask);
    //    task = schedswitchtask(oldtask);
    //    //    if (task != oldtask) {
    //    m_taskjmp(&task->m_tcb);
    //    }
}

#endif /* ZEROSCHED */

