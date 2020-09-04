#ifndef __SYS_ZEN_SCHED_TAO_H__
#define __SYS_ZEN_SCHED_TAO_H__

/* high-level zen scheduler interface */

#include <sys/zen/conf.h>

#if defined(__zen__)
extern void schedinit(void);
#endif
extern void schedyield(void);

#define TAO_SCHED_POLICY        TAO_LOW_LATENCY_SCHED
#define TAO_DESKTOP_SCHED       0
#define TAO_LOW_LATENCY_SCHED   1
#define TAO_SERVER_SCHED        2

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
#if (TAO_SCHED_POLICY == TAO_LOW_LATENCY_SCHED)
#define ZEN_TMR_HZ              125
#define ZEN_TMR_SLICE_MS        16
#elif (TAO_SCHED_POLICY == TAO_DESKTOP_SCHED)
#define ZEN_TMR_HZ              (125 / 2)
#define ZEN_TMR_SLICE_MS        32
#elif (TAO_SCHED_POLICY == TAO_SERVER_SCHED)
#define ZEN_TMR_HZ              (125 / 8)
#define ZEN_TMR_SLICE           64
#else
#errof TAO_SCHED_POLICY not specified in <sys/zen/sched/tao.h>
#endif
#endif /* (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED) */

#if (ZEN_TASK_SCHED == ZEN_ULE_TASK_SCHED)
#include <sys/zen/sched/ule.h>
#elif (ZEN_TASK_SCHED == ZEN_ULE_BVT_SCHED_SCHED)
#include <sys/zen/sched/bvt.h>
#endif

#endif /* __SYS_ZEN_SCHED_TAO_H__ */

