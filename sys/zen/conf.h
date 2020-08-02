#ifndef __SYS_ZEN_CONF_H__
#define __SYS_ZEN_CONF_H__

#include <sys/zen/ev.h>
#if defined(__v0__)
#include <sys/zen/bsp/v0.h>
#endif

/* compile-time options */
#define ZEN_HANG_ON_PANIC       1
#define ZEN_TASK_SCHED          ZEN_ULE_TASK_SCHED
#define ZEN_ULE_TASK_SCHED      1
#define ZEN_BVT_TASK_SCHED      2
#define ZEN_IP4_NET             1
#define ZEN_SMP                 1
#define ZEN_MAX_CPUS            32
#define ZEN_MAX_TASKS           256

/* system limits */
#define ZEN_OPEN_MAX            1024
#define ZEN_STREAM_MAX          1024
#define ZEN_SHM_MAX             1024
#define ZEN_EVQ_MAX             (MACH_PAGE_SIZE / ZEN_MAX_EV_SIZE)
#define ZEN_HOST_NAME_MAX       15
#define ZEN_LOGIN_NAME_MAX      15

/* system parameters */
#define ZEN_USR_STK_SIZE        (4 * MACH_PAGE_SIZE)
#define ZEN_USR_STK_MAX         (32 * MACH_PAGE_SIZE)
#define ZEN_SYS_STK_SIZE        (MACH_PAGE_SIZE)

#endif /* __SYS_ZEN_CONF_H__ */

