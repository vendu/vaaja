#ifndef __ZEN_TAO_H__
#define __ZEN_TAO_H__

#if defined(__v0__)
#include <zen/sys/v0.h>
#endif

m_thr_t                 g_taothrtab[MACH_MAX_THREADS]

#if defined(TAO_ULE_TASK_SCHED)
#include <zen/ule.h>
#endif
#if defined(TAO_BVT_TASK_SCHED)
#include <zen/bvt.h>
#endf

union taoschedparm {
#if defined(TAO_ULE_TASK_SCHED)
    struct taouleparm   ule;
#endif
#if defined(TAO_BVT_TASK_SCHED)
    struct taobvtparm   bvt;
#endf
};

#endif /* __ZEN_TAO_H__ */

