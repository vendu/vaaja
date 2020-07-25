#ifndef __SYS_ZEN_SCHED_TAO_H__
#define __SYS_ZEN_SCHED_TAO_H__

#include <sys/zen/conf.h>
#include <mt/tktlk.h>
#include <mach/types.h>

#if defined(__zen__)
extern void schedinit(void);
#endif
extern void schedyield(void);

struct  zenschedset {
    mttktlk                     lk;
    long                       *curmap;
    long                       *nextmap;
    long                       *idlemap;
    long                       *loadmap;
    struct zentask            **cur;
    struct zentask            **next;
    struct zentask            **idle;
};

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
#include <sys/zen/sched/ule.h>
#elif (ZEN_TASK_SCHED == ZEN_ULE_BVT_SCHED_SCHED)
#include <sys/zen/sched/bvt.h>
#endif

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
struct zenschedparm {
    m_word_t    unit;           // CPU-affinity
    m_word_t    sched;          // thread scheduler class
    m_word_t    intr;           // received interrupt
    m_word_t    runprio;        // current priority
    m_word_t    prio;           // base priority
    m_word_t    sysprio;        // kernel-mode priority
    m_word_t    niceval;        // priority adjustment
    m_word_t    state;          // thread state
    m_word_t    score;          // interactivity score
    m_word_t    slice;          // timeslice in ticks
    m_word_t    runtime;        // # of ticks run
    m_word_t    slptime;        // # of ticks slept voluntarily
    m_word_t    slptick;        // ID of tick when sleeping started
    m_word_t    ntick;          // # of scheduler ticks received
    m_word_t    lastrun;        // last tick we ran on
    m_word_t    firstrun;       // first tick we ran on
    m_word_t    ntickleft;      // # of remaining ticks of slice
    m_word_t    lasttick;       // real last tick for affinity
    m_adr_t     waitchan;       // wait channel
    m_time_t    timelim;        // wakeup time or deadline
};
#elif (ZEN_TASK_SCHED == ZEN_ULE_BVT_SCHED_SCHED)
/* per-thread scheduler parameters */
struct zenschedparm {
    m_uword_t   runtime;
    m_uword_t   slptime;
    long        warpofs;
    long        lastwarp;
};
#endif

#endif /* __SYS_ZEN_SCHED_TAO_H__ */

