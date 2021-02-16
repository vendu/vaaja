#ifndef __SYS_ZEN_SCHED_ULE_H__
#define __SYS_ZEN_SCHED_ULE_H__

#include <sys/zen/conf.h>

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)

#include <limits.h>
#include <env/cdefs.h>
#include <env/trix.h>
#include <mt/tkt.h>
#include <mach/types.h>
#include <zen/fastudiv.h>
#include <sys/zen/util.h>
#include <sys/zen/sys.h>
#include <sys/zen/zen.h>
#include <sys/zen/tmr.h>
#include <sys/zen/types.h>
#include <sys/zen/sched/types.h>
#include <sys/zen/sched/task.h>

#if (MACH_WORD_SIZE == 8)
#define SCHED_ULE_CLASS_WORDS       1
#elif (MACH_WORD_SIZE == 4)
#define SCHED_ULE_CLASS_WORDS       2
#endif

#define MACH_LONG_BITS  (CHAR_BIT * MACH_WORD_SIZE)
#if (MACH_LONG_BITS == 32)
#define __LONGBITSLOG2  5
#elif (MACH_LONG_BITS == 64)
#define __LONGBITSLOG2  6
#endif

#define SCHED_ULE_LVL0_DLS          (1U << 16)
#define SCHED_ULE_LVL1_DLS          (1U << 8)
#define SCHED_ULE_LVL2_DLS          (1U << 8)
#define SCHED_ULE_DL_KEYS           3

#define SCHED_ULE_DIVU16TAB_SIZE    65536

/* macros */
#define schedlkcpuntick(cpu)        (mtlktkt(&cpu->lk), (cpu)->ntick)
#define schedlkcpu(cpu)             (mtlktkt(&cpu->lk))
#define schedunlkcpu(cpu)           (mtunlktkt(&cpu->lk))
#define schedlktaskruntime(task)    (mtlktkt(&task->lk), (task)->runtime)
#define schedlktask(task)           (mtlktkt(&task->lk))
#define schedunlktask(task)         (mtunlktkt(&task->sched.lk))

#define schedcalctime(task)         ((task)->sched.ntick >> SCHED_ULE_TICK_SHIFT)
#define schedcalcticks(task)        (max((task)->sched.lastrun - (task)->sched.firstrun, kgethz()))
#define schedcalcnice(val)          (k_schednicetab[(val)].nice)
#define schedsetprio(task, type, p)                                     \
    ((task)->prio = schedcalcqueue(type, p))
#define schedsetprioincr(task, p, incr)                                 \
    ((task)->prio = schedcalcqueue(type, p) + (incr))
/* timeshare-tasks have interactivity scores */
#define schedistimeshare(sched)                                         \
    ((sched) >= SCHED_ULE_RESPONSIVE && (sched) <= SCHED_ULE_BATCH)
#define schedisinteract(score)                                          \
    ((score) < SCHED_ULE_SCORE_INT_LIM)
#define schedcalctrapqueue(p)                                           \
    (p)
#define schedcalcqueue(tupe, p)                                        \
    ((type == SCHED_ULE_REALTIME_RR)                                   \
     ? ((p) + schedclassminprio(SCHED_ULE_REALTIME))                   \
     : ((p) + schedclassminprio(type)))

/* interrupt priorities */
#define schedcalcintrprio(id)                                           \
    (SCHED_ULE_TRAP_PRIO + ((id) << SCHED_ULE_QUEUE_SHIFT))
#define SCHED_ULE_TRAP_RT_PRIO      schedcalcintrprio(0)
#define SCHED_ULE_TRAP_HID_PRIO     schedcalcintrprio(1)
#define SCHED_ULE_TRAP_AV_PRIO      schedcalcintrprio(2)
#define SCHED_ULE_TRAP_NET_PRIO     schedcalcintrprio(3)
#define SCHED_ULE_TRAP_DISK_PRIO    schedcalcintrprio(4)
#define SCHED_ULE_TRAP_MISC_PRIO    schedcalcintrprio(5)
#define SCHED_ULE_TRAP_SOFT_PRIO    schedcalcintrprio(6)

/*
 * in default configuration, we have
 *
 * SCHED_ULE_CLASS_PRIOS    64
 * SCHED_ULE_CLASS_QUEUES   SCHED_ULE_CLASS_PRIOS / 4 => 16
 */
/* task scheduler classes */
#define SCHED_ULE_CLASS_PRIOS       64      // # of priorities per class
#define SCHED_ULE_CLASS_QUEUES      (SCHED_ULE_CLASS_PRIOS >> SCHED_ULE_QUEUE_SHIFT)
#define SCHED_ULE_QUEUE_SHIFT       2
//#define SCHED_ULE_QUEUE_PRIOS       (1 << SCHED_ULE_QUEUE_SHIFT)
/* 'system' classes */
#define SCHED_ULE_DEADLINE          (-2) // deadline tasks
#define SCHED_ULE_REALTIME_RR       (-1)
#define SCHED_ULE_TRAP              0       // interrupt tasks
#define SCHED_ULE_REALTIME          1       // realtime threads
#define SCHED_ULE_SYSTEM            2
#define SCHED_ULE_SYSTEM_CLASSES    3       // # of system scheduler classes
/* timeshare classes */
#define SCHED_ULE_RESPONSIVE        3     // 'quick' timeshare tasks
#define SCHED_ULE_NORMAL            4     // 'normal' timeshare tasks
#define SCHED_ULE_BATCH             5           // batch tasks
#define SCHED_ULE_USER_CLASSES      3           // number of user/timeshare classes
#define SCHED_ULE_CLASSES           6           // # of user scheduler classes
#define SCHED_ULE_IDLE              SCHED_ULE_CLASSES // idle tasks
#define SCHED_ULE_QUEUES            (SCHED_ULE_CLASSES * SCHED_ULE_CLASS_QUEUES)
#define SCHED_ULE_TOTAL_QUEUES      (SCHED_ULE_QUEUES + SCHED_ULE_IDLE_QUEUES) // SCHED_ULE_IDLE too
#define SCHED_ULE_TAB_QUEUES        512
#define SCHED_ULE_NOCLASS           (-0x7f)

#if 0 /* FIXME: these will be handled in SCHED_ULE_TRAP */
/* fixed priorities */
#define SCHED_ULE_HID               0   // human interface devices (kbd, mouse)
#define SCHED_ULE_AUDIO             1   // audio synchronisation
#define SCHED_ULE_VIDEO             2   // video synchronisation
#define SCHED_ULE_INIT              3   // init; creation of new processes
#define SCHED_ULE_FIXED_PRIO_MIN    0
#endif

#define schedclassminprio(c)                                            \
    ((c) * SCHED_ULE_CLASS_PRIOS)
#define schedclassmaxprio(c)                                            \
    (schedclassminprio(c) + SCHED_ULE_CLASS_PRIOS - 1)
#define schedclassminqueue(c)                                           \
    ((c) * SCHED_ULE_CLASS_QUEUES)
/* priority organisation */
#define SCHED_ULE_IDLE_QUEUES       SCHED_ULE_CLASS_QUEUES
#define SCHED_ULE_SYSTEM_PRIO_MIN   schedclassminprio(SCHED_ULE_SYSTEM)
/* realtime priority limits */
#define SCHED_ULE_REALTIME_PRIO_MIN schedclassminprio(SCHED_ULE_REALTIME)
#define SCHED_ULE_REALTIME_PRIO_MAX schedclassmaxprio(SCHED_ULE_REALTIME)
/* interrupt priority limits */
#define SCHED_ULE_TRAP_PRIO_MIN     schedclassminprio(SCHED_ULE_TRAP)
#define SCHED_ULE_TRAP_PRIO_MAX     schedclassmaxprio(SCHED_ULE_TRAP)
/* timeshare priority limits */
#define SCHED_ULE_USER_PRIO_MIN     schedclassminprio(SCHED_ULE_RESPONSIVE)
/* positive nice values will not be mapped to SCHED_ULE_IDLE */
#define SCHED_ULE_USER_PRIO_MAX     (schedclassmaxprio(SCHED_ULE_BATCH) - SCHED_ULE_NICE_HALF)
#define SCHED_ULE_USER_RANGE        (SCHED_ULE_USER_PRIO_MAX - SCHED_ULE_USER_PRIO_MIN + 1)
/* batch priority limits */
#define SCHED_ULE_BATCH_PRIO_MIN    schedclassminprio(SCHED_ULE_BATCH)
#define SCHED_ULE_BATCH_PRIO_MAX    schedclassmaxprio(SCHED_ULE_BATCH)
#define SCHED_ULE_BATCH_RANGE       (SCHED_ULE_BATCH_PRIO_MAX - SCHED_ULE_BATCH_PRIO_MIN + 1)
/* nice limits */
#define SCHED_ULE_NICE_MIN          (-(SCHED_ULE_CLASS_QUEUES << 1))
#define SCHED_ULE_NICE_MAX          ((SCHED_ULE_CLASS_QUEUES << 1) - 1)
#define SCHED_ULE_NICE_RANGE        (SCHED_ULE_CLASS_QUEUES)
#define SCHED_ULE_NICE_HALF         (SCHED_ULE_CLASS_QUEUES >> 1)
/* idle priority limits */
#define SCHED_ULE_IDLE_PRIO_MIN     (SCHED_ULE_IDLE * SCHED_ULE_CLASS_PRIOS)
#define SCHED_ULE_IDLE_PRIO_MAX     (SCHED_ULE_IDLE_PRIO_MIN + SCHED_ULE_IDLE_QUEUES - 1)
#define SCHED_ULE_IDLE_RANGE        (SCHED_ULE_IDLE_PRIO_MAX - SCHED_ULE_IDLE_PRIO_MIN + 1)
/* highest and lowest priorities are reserved for nice */
/* we allow negative nice values to map to classes SCHEDREALTIME..SCHEDSYSTEM */
#define SCHED_ULE_PRIO_MIN          SCHED_ULE_USER_PRIO_MIN
#define SCHED_ULE_PRIO_MAX          SCHED_ULE_USER_PRIO_MAX
#define SCHED_ULE_PRIO_RANGE        (SCHED_ULE_PRIO_MAX - SCHED_ULE_PRIO_MIN + 1)
/* interactive priority limits */
#define SCHED_ULE_INT_PRIO_MIN      SCHED_ULE_USER_PRIO_MIN
//#define SCHED_ULE_INT_PRIO_MAX     (SCHED_ULE_BATCH_PRIO_MIN + SCHED_ULE_BATCH_PRIO_MAX - 1)
#define SCHED_ULE_INT_PRIO_MAX      SCHED_ULE_USER_PRIO_MAX
#define SCHED_ULE_INT_RANGE         (SCHED_ULE_INT_PRIO_MAX - SCHED_ULE_INT_PRIO_MIN + 1)

/* interactivity scoring */
/* interactivity scores are in the range [0, 128] */
#define SCHED_ULE_SCORE_MAX         128
/* minimum score to mark thread as interactive */
#define SCHED_ULE_SCORE_INT_LIM     32
/* half of maximum interactivity score */
#define SCHED_ULE_SCORE_HALF        (SCHED_ULE_SCORE_MAX >> 1)
/* number of seconds to keep cpu stats around */
#define SCHED_ULE_HISTORY_SECS      8
//#define SCHED_ULE_HISTORY_SIZE    (SCHED_ULE_HISTORY_MAX * (HZ << SCHED_ULE_TICK_SHIFT))
/* number of ticks to keep cpu stats around */
#define SCHED_ULE_HISTORY_TICKS     (SCHED_ULE_HISTORY_SECS * kgethz())
/* maximum number of ticks before scaling back */
#define SCHED_ULE_HISTORY_SIZE      (SCHED_ULE_HISTORY_TICKS + kgethz())
//#define SCHED_ULE_REC_TIME_DELTA       ((HZ << SCHED_ULE_TICK_SHIFT) / HZ)
#define SCHED_ULE_TICK_SHIFT        10
/* maximum number of sleep time + run time stored */
#define SCHED_ULE_REC_TIME_MAX      ((kgethz() << 2) * SCHED_ULE_HISTORY_SECS)
#define SCHED_ULE_REC_TIME_FORK_MAX ((kgethz() << 1) * SCHED_ULE_HISTORY_SECS)

#if 0 /* FIXME: these will be handled in ULE_TRAP */
/* fixed priorities */
#define ULE_HID             0           // human interface devices (kbd, mouse)
#define ULE_AUDIO           1           // audio synchronisation
#define ULE_VIDEO           2           // video synchronisation
#define ULE_INIT            3           // init; creation of new processes
#define ULE_FIXED_PRIO_MIN  0
#endif

#define SCHED_ULE_DEADLINE_WORDS                                        \
    (SCHED_ULE_LVL0_DLS / (CHAR_BIT * sizeof(long)))
#define SCHED_ULE_READY_WORDS                                           \
    max(SCHED_ULE_QUEUES / (CHAR_BIT * sizeof(long)), MACH_CL_SIZE / sizeof(long))
#define SCHED_ULE_IDLE_WORDS                                            \
    max(SCHED_ULE_IDLE_QUEUES / (CHAR_BIT * sizeof(long)), MACH_CL_SIZE / sizeof(long))
#define SCHED_ULE_LOAD_WORDS                                            \
    max(SCHED_ULE_QUEUES / (CHAR_BIT * sizeof(long)), MACH_CL_SIZE / sizeof(long))
#define SCHED_ULE_IDLECORE_WORDS                                        \
    max(ZEN_MAX_CPUS / (CHAR_BIT * sizeof(long)), MACH_CL_SIZE / sizeof(long))

extern struct divu16            k_fastu16divu16tab[SCHED_ULE_DIVU16TAB_SIZE];
extern m_word_t                 k_schedidlecoremap[SCHED_ULE_IDLECORE_WORDS];
extern struct zennice           k_schednicetab[SCHED_ULE_NICE_RANGE];
extern struct zennice          *k_schedniceptr;
extern struct zenschedset       k_schedreadyset;

#define schedclassminprio(c)    ((c) * SCHED_ULE_CLASS_PRIOS)
#define schedclassmaxprio(c)    (schedclassminprio(c)               \
                                 + SCHED_ULE_CLASS_PRIOS            \
                                 - 1)

/* based on sched_pctcpu_update from ULE */
static C_INLINE void
schedadjcpupct(struct zentask *task, long run)
{
    long            unit = task->sched.unit;
    struct zencpu  *cpu = &k_zensys.cputab[unit];
    long            tick = cpu->ntick;
    unsigned long   last = task->sched.lastrun;
    long            diff = tick - last;
    long            delta;
    long            ntick;
    long            val;
    long            div;

    if (diff >= SCHED_ULE_HISTORY_TICKS) {
        task->sched.ntick = 0;
        task->sched.firstrun = tick - SCHED_ULE_HISTORY_TICKS;
    } else {
        unsigned long first = task->sched.firstrun;

        delta = tick - first;
        if (delta >= SCHED_ULE_HISTORY_SIZE) {
            ntick = task->sched.ntick;
            div = last - first;
            val = tick - SCHED_ULE_HISTORY_TICKS;
            last -= val;
            ntick = fastu16divu16(ntick, div, k_fastu16divu16tab);
            ntick *= last;
            task->sched.firstrun = val;
            task->sched.ntick = ntick;
        }
    }
    if (run) {
        ntick = diff >> SCHED_ULE_TICK_SHIFT;
        task->sched.ntick = ntick;
    }
    task->sched.lastrun = tick;

    return;
}

static C_INLINE void
schedswapqueues(void)
{
    struct zenschedset *set = &k_schedreadyset;

    mtlktkt(&set->lk);
    set->next = set->cur;
    set->cur = set->next;
    set->nextmap = set->curmap;
    set->curmap = set->nextmap;
    mtunlktkt(&set->lk);

    return;
}

static C_INLINE volatile struct zencpu *
schedfindidlecore(long unit, long *retcore)
{
    volatile struct zencpu *cpu = &k_zensys.cputab[unit];
    long                    nunit = ZEN_MAX_CPUS;
    long                    ndx = 0;
    long                    val = 0;

    for (ndx = 0 ; ndx < nunit ; ndx++) {
        if (ndx != unit && bitset(k_schedidlecoremap, ndx)) {
            *retcore = ndx;

            return cpu;
        }
    }

    return NULL;
}

static C_INLINE void
schedsetnice(struct zentask *task, long val)
{
    long            pid = task->id;
    struct zenproc *proc = k_zensys.proctab[pid];
    long            niceval;

    val = max(-20, val);
    val = min(19, val);
    niceval = k_schedniceptr[val].nice;
    proc->nice = niceval;
    proc->niceval = val;

    return;
}

/* CHECKED against sched_interact_score() in ULE :) */
static C_INLINE long
schedcalcscore(struct zentask *task)
{
    long run = task->sched.runtime;
    long slp = task->sched.slptime;
    long div;
    long res;
    long tmp;

    if (SCHED_ULE_SCORE_INT_LIM <= SCHED_ULE_SCORE_HALF
        && run >= slp) {
        res = SCHED_ULE_SCORE_HALF;
        task->sched.score = res;

        return res;
    }
    if (run > slp) {
#if (SCHED_ULE_SCORE_HALF == 64)
        run >>= 6;
#else
        run = fastu16divu16(run, SCHED_ULE_SCORE_HALF, k_fastu16divu16tab);
#endif
        res = SCHED_ULE_SCORE_MAX;
        div = max(1, run);
        tmp = fastu16divu16(slp, div, k_fastu16divu16tab);
        res -= tmp;
        task->sched.score = res;

        return res;
    }
    if (slp > run) {
#if (SCHED_ULE_SCORE_HALF == 64)
        slp >>= 6;
#else
        slp = fastu16divu16(slp, SCHED_ULE__HALFSCORE, s);
#endif
        div = max(1, slp);
        res = fastu16divu16(run, div, k_fastu16divu16tab);
        task->sched.score = res;

        return res;
    }
    /* run == slp */
    if (run) {
        res = SCHED_ULE_SCORE_HALF;
        task->sched.score = res;

        return res;
    }
    task->sched.score = 0;

    /* run == 0 && slp == 0 */
    return 0;
}

static C_INLINE uint32_t
schedcalcuserprio(struct zentask *task)
{
    uint32_t time = schedcalctime(task);
    uint32_t val = schedcalcticks(task);
    uint32_t res;

    /*
     * round val up to next multiple of SCHED_ULE_PRIO_RANGE,
     * then divide by SCHED_ULE_PRIO_RANGE; i'm cheating big time =)
     */
    val += SCHED_ULE_PRIO_RANGE - 1;
    val = fastu16divu16(val, SCHED_ULE_PRIO_RANGE, k_fastu16divu16tab);
    /* divide runtime by the result */
    res = fastu16divu16(time, (uint16_t)val, k_fastu16divu16tab);

    return res;
}

/* based on sched_priority() from ULE */
static C_INLINE void
schedcalcprio(struct zentask *task)
{
    long score = schedcalcscore(task);
    long nice = schedcalcnice(task->proc->niceval);
    long runprio = task->sched.runprio;
    long prio;
    long ntick;
    long delta;

    score += nice;
    score = max(0, score);
    if (score < SCHED_ULE_SCORE_INT_LIM) {
        prio = SCHED_ULE_INT_PRIO_MIN;
        delta = SCHED_ULE_INT_RANGE;
#if (SCHED_ULE_SCORE_INT_LIM == 32)
        delta >>= 5;
#else
        delta = fastu16divu16(delta, SCHED_ULE_SCORE_INT_LIM, k_fastu16divu16tab);
#endif
        delta *= score;
        prio += delta;
    } else {
        ntick = task->sched.ntick;
        prio = SCHED_ULE_USER_PRIO_MIN;
        if (ntick) {
            delta = schedcalcuserprio(task);
            delta = min(delta, SCHED_ULE_USER_RANGE - 1);
            prio += delta;
        }
        prio += nice;
        prio = min(runprio, prio);
    }
    task->sched.prio = prio;

    return;
}

/* based on sched_interact_update() in ULE :) */
/*
 * enforce maximum limit of scheduling history kept; call after either runtime
 * or slptime is adjusted
 */
static C_INLINE void
schedadjintparm(struct zentask *task)
{
    long run = task->sched.runtime;
    long slp = task->sched.slptime;
    long sum = run + slp;

    if (sum < SCHED_ULE_REC_TIME_MAX) {

        return;
    }
    if (sum > 2 * SCHED_ULE_REC_TIME_MAX) {
        if (run > slp) {
            task->sched.runtime = SCHED_ULE_REC_TIME_MAX;
            task->sched.slptime = 1;
        } else {
            task->sched.runtime = 1;
            task->sched.slptime = SCHED_ULE_REC_TIME_MAX;
        }

        return;
    }
    if (sum > (SCHED_ULE_REC_TIME_MAX >> 3) * 9) {
        /* exceeded by more than 1/8th, divide by 2 */
        run >>= 1;
        slp >>= 1;
    } else {
        /* multiply by 3 / 4; this gives us less than 0.85 * SCHED_ULE_REC_TIME_MAX */
        run >>= 2;
        slp >>= 2;
        run *= 3;
        slp *= 3;
    }
    task->sched.runtime = run;
    task->sched.slptime = slp;

    return;
}

/* based on sched_interact_fork() in ULE */
static C_INLINE void
schedadjforkintparm(struct zentask *task)
{
    long    run = task->sched.runtime;
    long    slp = task->sched.slptime;
    long    ratio;
    long    run2;
    long    slp2;
    long    sum = run + slp;

    if (sum > SCHED_ULE_REC_TIME_FORK_MAX) {
#if (SCHED_ULE_HISTORY_SIZE <= 8)
        /* multiply run and slp by 3 / 8 */
        run2 = run;
        slp2 = slp;
        run >>= 3;
        slp >>= 3;
        run2 >>= 2;
        slp2 >>= 2;
        run += run2;
        slp += slp2;
#else
        ratio = fastu16divu16(sum, SCHED_ULE_REC_TIME_FORK_MAX, k_fastu16divu16tab);
        run = fastu16divu16(run & 0xffff, ratio & 0xffff, k_fastu16divu16tab);
        slp = fastu16divu16(slp & 0xffff, ratio & 0xffff, k_fastu16divu16tab);
#endif
        task->sched.runtime = run;
        task->sched.slptime = slp;
    }

    return;
}

/* applied for time-share tasks of classes SCHED_ULE_NORMAL and SCHED_ULE_BATCH */
/* return value is new priority */
static C_INLINE long
schedcalcintparm(struct zentask *task, long *retscore)
{
    long range = SCHED_ULE_INT_RANGE;
    long res = 0;
    long nice = task->proc->niceval;
    long score;
    long diff;
    long ntick;
    long tickhz;
    long total;
    long div;
    long tmp;

    score = schedcalcscore(task);
    score += nice;
    score = max(0, score);
    if (score < SCHED_ULE_SCORE_INT_LIM) {
        /* map interactive tasks to priorities SCHED_ULE_RTMIN..SCHED_ULE_BATCH_PRIO_MIN */
        res = SCHED_ULE_INT_PRIO_MIN;
#if (SCHED_ULE_SCORE_INT_LIM == 32)
        range >>= 5;
#else
        range = fastu16divu16(range, SCHED_ULE_SCORE_INT_LIM, k_fastu16divu16tab);
#endif
        range *= score;
        res += range;
    } else {
        ntick = task->sched.ntick;
        if (ntick) {
            /* map noninteractive tasks to priorities SCHED_ULE_SYSTEM..SCHED_ULE_BATCH */
            total = task->sched.lastrun - task->sched.firstrun;
            tickhz = ntick >> SCHED_ULE_TICK_SHIFT;
            diff = SCHED_ULE_USER_PRIO_MAX + SCHED_ULE_NICE_HALF;
            res = SCHED_ULE_USER_PRIO_MIN - SCHED_ULE_NICE_HALF;
            total = max(total, kgethz());
            range = diff - res + 1;
            //            tmp = roundup(total, range);
            tmp = _roundup16b(total, range);
            res += nice;
            div = fastu16divu16(total, tmp, k_fastu16divu16tab);
            range--;
            total = fastu16divu16(tickhz, div, k_fastu16divu16tab);
            diff = min(total, range);
            res += diff;
        }
    }
    *retscore = score;

    return res;
}

/* based on sched_wakeup() from ULE :) */
static C_INLINE void
taskwakeup(struct zentask *task)
{
    long            unit = task->sched.unit;
    struct zencpu  *cpu = &k_zensys.cputab[unit];
    long            sched = task->sched.sched;
    long            slptick = task->sched.slptick;
    long            slp;
    long            tick;
    long            ntick;
    long            diff;
#if (ZEN_SMP)
    long            core;
#endif

    task->sched.slptick = 0;
    if (slptick) {
        tick = cpu->ntick;
        if (slptick != tick) {
            diff = tick - slptick;
            slp = task->sched.slptime;
            diff <<= SCHED_ULE_TICK_SHIFT;
            task->sched.slptime = diff;
            schedadjintparm(task);
            schedadjcpupct(task, 0);
        }
    }
    task->sched.slice = 0;
    if (schedistimeshare(sched)) {
        schedcalcprio(task);
    }
#if (ZEN_SMP) && 0
    unit = schedfindidlecore(unit, &core);
#endif
    task->state = ZEN_TASK_READY;
    task->sched.unit = unit;
    k_jmptask(task->m_tcb);
#if 0
    schedsetcpu(&k_zensys.cputab[unit]);
    schedsetready(task, unit);
#endif
    /* FIXME: sched_setpreempt() */
}

#endif /* (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED) */

#endif /* __SYS_ZEN_SCHED_ULE_H__ */

