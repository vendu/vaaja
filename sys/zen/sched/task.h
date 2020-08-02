#ifndef __SYS_ZEN_SCHED_TASK_H__
#define __SYS_ZEN_SCHED_TASK_H__

#include <mach/types.h>
#include <sys/zen/types.h>
#include <sys/zen/vm.h>
#include <sys/zen/sched/tao.h>

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

C_NORETURN C_INLINE void
k_jmptask(struct m_thr *thr)
{
    while (1) {
        ;
    }
}

#endif /* __SYS_ZEN_SCHED_TASK_H__ */

