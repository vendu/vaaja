#ifndef __ZEN_SCHED_TAO_H__
#define __ZEN_SCHED_TAO_H__

#include <sys/zen/conf.h>

#if defined(TAO_ULE_TASK_SCHED)
#include <sys/zen/sched/ule.h>
#endif
#if defined(TAO_BVT_TASK_SCHED)
#include <sys/zen/ule/bvt.h>
#endif

union taoschedparm {
#if defined(TAO_ULE_TASK_SCHED)
    struct taouleparm   ule;
#endif
#if defined(TAO_BVT_TASK_SCHED)
    struct taobvtparm   bvt;
#endif
};

#endif /* __ZEN_SCHED_TAO_H__ */

