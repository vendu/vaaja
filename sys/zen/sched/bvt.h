#ifndef __SYS_ZEN_SCHED_BVT_H__
#define __SYS_ZEN_SCHED_BVT_H__

#include <sys/zen/conf.h>

#if (ZEN_TASK_SCHED == ZEN_BVT_TASK_SCHED)

#include <sys/zen/task.h>
#include <mach/types.h>

#define TAO_MIN_TIME            1
#define TAO_WARP_LIM            8
#define TAO_WARP_DELAY          (V0_HZ / 4)

#define taoadjruntime(runtm)    (max(runtm, V0_MIN_TIME))
#define taowarpvtime(parm)                                              \
    ((parm)->vtime                                                      \
     - ((((parm)->flg & TAO_WARP_BIT)                                   \
         && (parm)->warpofs < TAO_WARP_MAX                              \
         && (parm)->lastwarp >= TAO_WARP_DELAY)                         \
        ? (parm)->warpofs                                               \
        : 0))

/* global scheduler attributes */
struct taoschedparm {
    m_uword_t   vtime;
    m_uword_t   minvtime;
    m_word_t    warplim;
    m_word_t    tickhz;
};

#endif /* (ZEN_TASK_SCHED == ZEN_BVT_TASK_SCHED) */

#endif /* __SYS_ZEN_SCHED_BVT_H__ */

