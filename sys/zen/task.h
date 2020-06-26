#ifndef __ZEN_TASK_H__
#define __ZEN_TASK_H__

#include <zen/conf.h>
#include <zen/types.h>
#include <zen/sched/tao.h>
#include <mach/param.h>
#include <zen/types.h>

/*
 * NOTES
 * -----
 * #define TASK_SELF    (-1)
 * pid_t                sys_proc(flg);
 * void                 taskdt(tid);
 * void                 taskat(tid);
 * pid_t                taskrm(tid);
 */

/* system threads */
#define ZEN_KERNEL_THREAD       0       // kernel
#define ZEN_INIT_THREAD         1       // init
#define ZEN_PAGE_THREAD         2       // page-daemon
#define ZEN_BUF_THREAD          3       // buf-daemon
#define ZEN_SYS_THREADS         4       // # of predefined system threads
#define ZEN_USR_PID_MIN         ZEN_SYS_THREADS
#define ZEN_USR_PID_MAX         (MACH_MAX_THREADS - 1)

/* process states */
#define ZEN_PROC_NEW            0
#define ZEN_PROC_READY          1
#define ZEN_PROC_RUN            2
#define ZEN_PROC_WAIT           3
#define ZEN_PROC_SLEEP          4
#define ZEN_PROC_ZOMBIE         5
#define ZEN_PROC_EXIT           6
#define ZEN_PROC_IDLE           7

//#define _zenproclkpageq(proc)   mtlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
//#define _zenprocunlkpageq(proc) mtunlkbit(&proc->pageq, MT_MEM_LK_BIT_OFS)
struct zenproc {
    m_word_t            flags;
    union taoschedparm  sched;
    volatile void      *pageq;  // process page queue
    struct m_tcb       *thrtab[MACH_PROC_THREADS];
};

struct zentask {
    struct m_tcb        tcb;    // task control block
    m_word_t            pgrp;   // parent/group process ID
    m_word_t            state;  // task state
    m_word_t            flags;  // task flags
    union taoschedparm  sched;  // task scheduler parameters
};

#endif /* __ZEN_TASK_H__ */

