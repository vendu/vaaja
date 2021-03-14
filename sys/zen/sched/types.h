#ifndef __SYS_ZEN_SCHED_TYPES_H__
#define __SYS_ZEN_SCHED_TYPES_H__

#include <sys/zen/conf.h>
#include <mach/types.h>
#include <stdint.h>

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
struct  zenschedset {
    mttktlk                     lk;
#if defined(__v0__)
    m_word_t                    pad;
#endif
    m_word_t                   *curmap;
    m_word_t                   *nextmap;
    m_word_t                   *idlemap;
    m_word_t                   *loadmap;
    struct zentask            **cur;
    struct zentask            **next;
    struct zentask            **idle;
};

struct zenschedparm {
    m_word_t                    unit;           // CPU-affinity
    m_word_t                    policy;         // thread scheduler class
    m_word_t                    intr;           // received interrupt
    m_word_t                    runprio;        // current priority
    m_word_t                    prio;           // base priority
    m_word_t                    sysprio;        // kernel-mode priority
    m_word_t                    niceval;        // priority adjustment
    m_word_t                    state;          // thread state
    m_word_t                    score;          // interactivity score
    m_word_t                    slice;          // timeslice in ticks
    m_word_t                    runtime;        // # of ticks run
    m_word_t                    slptime;        // # of ticks slept voluntarily
    m_word_t                    slptick;        // ID of tick when sleeping started
    m_word_t                    ntick;          // # of scheduler ticks received
    m_word_t                    lastrun;        // last tick we ran on
    m_word_t                    firstrun;       // first tick we ran on
    m_word_t                    ntickleft;      // # of remaining ticks of slice
    m_word_t                    lasttick;       // real last tick for affinity
    m_adr_t                     waitchan;       // wait channel
    m_time_t                    timelim;        // wakeup time or deadline
};

#elif (ZEN_TASK_SCHED == ZEN_BVT_SCHED)

/* per-thread scheduler parameters */
struct zenschedparm {
    m_uword_t                   runtime;
    m_uword_t                   slptime;
    m_word_t                    warpofs;
    m_word_t                    lastwarp;
};

#elif (ZEN_TASK_SCHED == ZEN_UNI_SCHED)

/* per-thread scheduler parameters */
struct zenschedparm {
    m_word_t                    policy;
    m_word_t                    defpolicy;
    m_word_t                    prio;
    m_word_t                    defprio;
    m_word_t                    niceval;
    m_word_t                    slice;
};

#endif /* ZEN_TASK_SCHED */

//#define _zenproclkpageq(proc)   mtlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
//#define _zenprocunlkpageq(proc) mtunlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
struct zenproc {
    struct m_thr                m_tcb;  // task control block
    struct k_zenvars           *zenvars;
    struct zenvmqueue           pagequeue;
    zenpid_t                    ppid;
    zenpid_t                    pid;
    m_word_t                    flags;
    m_word_t                    nice;
    m_word_t                    niceval;
    struct zenschedparm         sched;
#if defined(__v0__)
    m_word_t                    pad;
#endif
    struct m_thr               *thrtab[ZEN_MAX_TASKS];
};

struct zentask {
    struct m_thr               *m_tcb;
    struct zenproc             *proc;
    zenpid_t                    id;
    zenpid_t                    pgrp;   // parent/group process ID
    m_word_t                    state;  // task state
    m_word_t                    flags;    // task flags
    struct zentask             *prev;
    struct zentask             *next;
    struct zenschedparm         sched;
};

struct zennice {
    int_fast8_t                 nice;
    int_fast8_t                 slice;
};

#endif /* __SYS_ZEN_SCHED_TYPES_H__ */

