/* 32-bit time_t values */
#define scheddlkey0(dl)             (((dl) >> 16) & 0xffff)
#define scheddlkey1(dl)             (((dl) >> 8) & 0xff)
#define scheddlkey2(dl)             ((dl) & 0xff)

struct divu16               k_fastu16divu16tab[SCHED_ULE_DIVU16TAB_SIZE];
static struct zenproc      *k_proczombietab[ZEN_MAX_TASKS];
static struct zentask      *k_schedstoppedtab[ZEN_MAX_TASKS];
static struct zentask      *k_schedreadytab0[SCHED_ULE_QUEUES];
static struct zentask      *k_schedreadytab1[SCHED_ULE_QUEUES];
static struct zentask      *k_schedidletab[SCHED_ULE_CLASS_QUEUES];
static m_word_t             k_schedreadymap0[SCHED_ULE_READY_WORDS];
static m_word_t             k_schedreadymap1[SCHED_ULE_READY_WORDS];
static m_word_t             k_schedidlemap[SCHED_ULE_IDLE_WORDS];
/* SCHED_ULE_IDLE queues are not included in SCHED_ULE_CLASS_QUEUES */
static m_word_t             k_schedloadmap[SCHED_ULE_LOAD_WORDS];
static m_word_t             k_scheddeadlinemap[SCHED_ULE_CLASS_WORDS];
m_word_t                    k_schedidlecoremap[SCHED_ULE_IDLECORE_WORDS];
static struct zentasktab    k_scheddeadlinetab[SCHED_ULE_CLASS_QUEUES];
struct zennice             *k_schedniceptr = &k_schednicetab[SCHED_ULE_NICE_HALF];
struct zenschedset          k_schedreadyset;

static void
schedinitset(void)
{
    long                lim = ZEN_MAX_CPUS;
    struct zenschedset *set = &k_schedreadyset;
    long                id;

    lim >>= MACH_LONG_BITS_LOG2;
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
    kprintf("SCHED_ULE_HISTORY_SIZE == %d\n", SCHED_ULE_HISTORY_SIZE);
    fastu16divu16gentab(k_fastu16divu16tab, SCHED_ULE_DIVU16TAB_SIZE);
    schedinitset();

    return;
}

#define DEQ_SINGLE_TYPE
#define DEQ_TYPE struct zentask
#include <zero/deq.h>

static void
schedsetdeadline(struct zentask *task)
{
    struct zentasktab **l0tab;
    struct zentask    **tab;
    time_t              deadline = task->sched.timelim;
    unsigned long       key0 = scheddlkey0(deadline);
    unsigned long       key1 = scheddlkey1(deadline);
    unsigned long       key2 = scheddlkey2(deadline);
    m_word_t           *map = k_scheddeadlinemap;
    void               *ptr = NULL;
    void               *vptr = NULL;
    long                fail = 0;
    void               *ptab[SCHED_ULE_DL_KEYS - 1] = { NULL, NULL };

    mtlktkt(&k_scheddeadlinetab[key0].tkt);
    ptr = k_scheddeadlinetab[key0].tab;
    if (!ptr) {
        ptr = kmalloc(SCHED_ULE_LVL0_DLS * sizeof(struct zentasktab));
        if (ptr) {
            kbzero(ptr, SCHED_ULE_LVL0_DLS * sizeof(struct zentasktab));
        }
        ptab[0] = ptr;
        k_scheddeadlinetab[key0].tab = ptr;
        vptr = ptr;
    }
    if (!ptr) {
        kdebug("cannot allocata deadline L0-table", ptab[0]);
    }
    ptr = ((struct zentasktab *)vptr)[key1].tab;
    if (!ptr) {
        ptr = kmalloc(SCHED_ULE_LVL1_DLS * sizeof(struct zentasktab));
        if (ptr) {
            kbzero(ptr, SCHED_ULE_LVL1_DLS * sizeof(struct zentasktab));
        }
        ptab[1] = ptr;
        ((struct zentasktab *)vptr)[key1].tab = ptr;
        vptr = ptr;
    }
    if (!ptr) {
        kdebug("cannot allocata deadline L0-table", ptab[0]);
    }
    ptr = ((struct zentasktab *)vptr)[key2].tab;
    if (!ptr) {
        ptr = kmalloc(SCHED_ULE_LVL2_DLS * sizeof(struct zentasktab));
        if (ptr) {
            kbzero(ptr, SCHED_ULE_LVL1_DLS * sizeof(struct zentasktab));
        }
        ptab[2] = ptr;
        ((struct zentasktab *)vptr)[key2].tab = ptr;
    }
    if (!ptr) {
        kdebug("cannot allocata deadline L0-table", ptab[0]);
    }
    deqappend(task, ptr);
    mtunlktkt(&k_scheddeadlinetab[key0].tkt);

    return;
}

static void
schedsetready(struct zentask *task)
{
    long                    sched = task->sched.sched;
    long                    prio = task->sched.prio;
    struct zenschedset     *set = &k_schedreadyset;
    struct zentask        **queue;
    m_word_t               *map;
    long                    score;
    long                    type;
    long                    ndx;
    long                    qid;
    long                    load;
    long                    lim;
    long                    flg;

    type = sched;
    if (type == SCHED_ULE_DEADLINE) {
        /* SCHED_ULE_DEADLINE */
        schedsetdeadline(task);

        return;
    } else if (type <= SCHED_ULE_SYSTEM) {
        /*
         * SCHED_ULE_REALTIME_RR, SCHED_ULE_TRAP, SCHED_ULE_REALTIME
         * or SCHED_ULE_SYSTEM
         */
        if (type == SCHED_ULE_TRAP) {
            qid = schedcalctrapqueue(prio);
        } else if (type == SCHED_ULE_REALTIME_RR
                   || type == SCHED_ULE_REALTIME) {
            qid = schedcalcqueue(type, prio);
        } else {
            /* SCHED_ULE_SYSTEM */
            lim = schedclassmaxprio(SCHED_ULE_SYSTEM);
            if (prio < lim) {
                prio++;
                task->sched.prio = prio;
            }
            qid = schedcalcqueue(SCHED_ULE_SYSTEM, prio);
        }
        mtlktkt(&set->lk);
        map = set->curmap;
        queue = set->cur;
        setbit(map, qid);
    } else if (type != SCHED_ULE_IDLE) {
        /* SCHED_ULE_RESPONSIVE, SCHED_ULE_NORMAL, SCHED_ULE_BATCH */
        flg = task->flg;
        score = 0;
        if (type == SCHED_ULE_RESPONSIVE) {
            lim = schedclassmaxprio(SCHED_ULE_RESPONSIVE);
            prio++;
            if (prio == lim) {
                prio = 0;
                type = SCHED_ULE_NORMAL;
            }
        } else if (flg & ZEN_TASK_HAS_INPUT) {
            /* boost user-interrupt task to highest priority */
            sched = SCHED_ULE_RESPONSIVE;
            prio = schedclassminprio(SCHED_ULE_RESPONSIVE);
        } else {
            /* SCHED_ULE_NORMAL or SCHED_ULE_BATCH; calculate timeshare priority */
            prio = schedcalcintparm(task, &score);
        }
        task->sched.sched = type;
        task->sched.prio = prio;
        qid = schedcalcqueue(type, prio);
        if (schedisinteract(score)) {
            /* if interactive, insert onto current queue */
            mtlktkt(&set->lk);
            map = set->curmap;
        } else {
            /* if not interactive, insert onto next queue */
            mtlktkt(&set->lk);
            map = set->nextmap;
        }
        queue = set->cur;
        setbit(map, qid);
    } else {
        /* SCHED_ULE_IDLE */
        lim = SCHED_ULE_CLASSES * SCHED_ULE_CLASS_PRIOS + SCHED_ULE_IDLE_QUEUES - 1;
        if (prio < lim) {
            prio++;
            task->sched.prio = prio;
        }
        qid = schedcalcqueue(SCHED_ULE_IDLE, prio);
        ndx = qid - schedclassminqueue(SCHED_ULE_IDLE);
        mtlktkt(&set->lk);
        map = set->idlemap;
        queue = set->cur;
        setbit(map, ndx);
    }
    load = set->loadmap[qid];
    load++;
    if (sched == SCHED_ULE_REALTIME_RR) {
        deqappend(task, queue);
    } else {
        /* SCHED_ULE_REALTIME without SCHED_REALTIME_RR */
        deqpush(task, queue);
    }
    set->loadmap[qid] = load;
    mtunlktkt(&set->lk);

    return;
}

static void
schedsetsleep(struct zentask *task)
{
    kpanic("schedsetsleep() not implemented yet", SIGSYS);
}

static void
schedsetstopped(struct zentask *task)
{
    long id = task->id;

    k_schedstoppedtab[id] = task;

    return;
}

static void
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
    long                    unit = curtask->sched.unit;
    long                    state = (curtask) ? curtask->state : -1;
    struct zenschedset     *set = &k_schedreadyset;
    struct zentask         *task;
    struct zentask         *next;
    struct zentask        **queue;
    m_word_t               *map;
    long                    val;
    long                    ndx;
    long                    ofs;
    long                    lim;
    long                    loop;

    do {
        if (curtask) {
            /* kernel initialisations done */
            switch (state) {
                case ZEN_TASK_NEW:
                case ZEN_TASK_READY:
                    schedsetready(curtask);

                    break;
                case ZEN_TASK_SLEEP:
                    schedsetsleep(curtask);

                    break;
                case ZEN_TASK_STOPPED:
                    schedsetstopped(curtask);

                    break;
                case ZEN_TASK_ZOMBIE:
                    schedsetzombie(curtask->proc);

                    break;
                default:
                    kdebug("invalid task state", curtask); /* FIXME: error # */

                    break;
            }
            do {
                loop = 1;
                do {
                    /* loop over current and next priority-queues */
                    lim = SCHED_ULE_CLASS_QUEUES;
                    lim >>= MACH_LONG_BITS_LOG2;
                    mtlktkt(&set->lk);
                    map = set->curmap;
                    queue = &set->cur[SCHED_ULE_TRAP_PRIO_MIN];
                    for (ndx = 0 ; ndx < lim ; ndx++) {
                        val = map[ndx];
                        if (val) {
                            ofs = ndx * CHAR_BIT * sizeof(long);
                            ndx = tzerol(val);
                            ofs += ndx;
                            if (ofs < SCHED_ULE_CLASS_QUEUES) {
                                queue += ofs;
                                task = deqpop(queue);
                                if (!queue) {
                                    m_clrbit((m_atomic_t *)map, ofs);
                                }
                                mtunlktkt(&set->lk);

                                k_jmptask(task->m_tcb);
                            }
                        }
                    }
                    /* if no task found during the first iteration, switch queues */
                    schedswapqueues();
                    mtunlktkt(&set->lk);
                } while (loop--);
                /* if both current and next queues are empty, look for an idle task */
                lim = SCHED_ULE_IDLE_QUEUES;
                lim >>= MACH_LONG_BITS_LOG2;
                mtlktkt(&set->lk);
                map = set->idlemap;
                queue = k_schedidletab;
                for (ndx = 0 ; ndx < lim ; ndx++) {
                    val = map[ndx];
                    if (val) {
                        ofs = ndx * CHAR_BIT * sizeof(long);
                        ndx = tzerol(val);
                        ofs += ndx;
                        if (ofs < SCHED_ULE_IDLE_QUEUES) {
                            queue += ofs;
                            task = deqpop(queue);
                            if (!queue) {
                                m_clrbit((m_atomic_t *)map, ofs);
                            }
                            mtunlktkt(&set->lk);

                            k_jmptask(task->m_tcb);
                        }
                    }
                }
                /* FIXME: try to pull threads from other cores here */
                //        task = taskpull(unit);
                /* mark the core as idle */
                map = set->idlemap;
                setbit(map, unit);
                mtunlktkt(&set->lk);
                kintron();
                m_waitint();
            } while (1);
            k_jmptask(task->m_tcb);
        }
    } while (1);
}

#if 0
static void
schedyield(void)
{
    struct zentask *curtask = k_getcurtask();

    schedswitchtask(curtask);
}
#endif

C_NORETURN void
schedyield(void)
{
    struct zentask *oldtask = k_getcurtask();

    schedswitchtask(oldtask);
}

