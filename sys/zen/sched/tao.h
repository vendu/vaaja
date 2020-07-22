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

#endif /* __SYS_ZEN_SCHED_TAO_H__ */

