#ifndef __SYS_ZEN_SCHED_TASK_H__
#define __SYS_ZEN_SCHED_TASK_H__

#include <mach/types.h>
#include <sys/zen/types.h>
#include <sys/zen/sched/ule.h>

/*
 * NOTES
 * -----
 * #define TASK_SELF    (-1)
 * pid_t                sys_proc(flg);
 * void                 taskdt(tid);
 * void                 taskat(tid);
 * pid_t                taskrm(tid);
 */

/* process states */
#define ZEN_TASK_NEW            0
#define ZEN_TASK_READY          1
#define ZEN_TASK_RUN            2
#define ZEN_TASK_WAIT           3
#define ZEN_TASK_SLEEP          4
#define ZEN_TASK_STOPPED        5
#define ZEN_TASK_ZOMBIE         6
#define ZEN_TASK_EXIT           7
#define ZEN_TASK_IDLE           8

/* process flags */
#define ZEN_TASK_HAS_INPUT      (1 << 0)

//#define _zenproclkpageq(proc)   mtlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
//#define _zenprocunlkpageq(proc) mtunlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
struct zenproc {
    struct m_thr            m_tcb;  // task control block
    m_word_t                flg;
    volatile void          *pageq;  // process page queue
    zenpid_t                ppid;
    zenpid_t                pid;
    m_word_t                nice;
    m_word_t                niceval;
    struct zenschedparm     sched;
    struct m_thr           *thrtab[ZEN_MAX_TASKS];
};

struct zentask {
    struct m_thr           *m_tcb;
    struct zenproc         *proc;
    zenpid_t                id;
    zenpid_t                pgrp;   // parent/group process ID
    m_word_t                state;  // task state
    m_word_t                flg;    // task flags
    struct zentask         *prev;
    struct zentask         *next;
    struct zenschedparm     sched;
};

#endif /* __SYS_ZEN_TASK_H__ */

