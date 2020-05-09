#ifndef __ZEN_CONF_H__
#define __ZEN_CONF_H__

#if defined(__v0__)
#include <zen/sys/v0.h>
#endif

#define ZEN_DEF_TASK_SCHED      ZEN_ULE_TASK_SCHED
#define ZEN_ULE_TASK_SCHED      1
#define ZEN_BVT_TASK_SCHED      2
#define ZEN_IP4_NET             1

/* system parameters */
#define ZEN_USR_STK_SIZE        (2 * MACH_PAGE_SIZE)
#define ZEN_SYS_STK_SIZE        (MACH_PAGE_SIZE)

#endif /* __ZEN_CONF_H__ */

