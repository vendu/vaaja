#ifndef ZEN_CONF_H
#define ZEN_CONF_H

#define ZEN_GFX_DECK
#define ZEN_TTY
#define ZEN_VT
#define ZEN_PIX_FORMAT          GFX_RGB444

#define ZEN_SIMULATION
/* in simulation mode, th kernel runs on top of a system instead of being standalone */
#if defined(ZEN_SIMULATION)
#define ZEN_RUN_ON_UX           1   // execute on top of a Unix-like
#define ZEN_USE_PTHREAD         1   // virtual kernel with POSIX Threads
#define ZEN_USE_STDIO           1   // use standard C library I/O (simulation)
#endif /* defined(ZEN_SIMULATION) */

/* compile-time options */
#define ZEN_HANG_ON_PANIC       1
#if defined(__voima__)          // fixed-priority scheduler for the consoles
#define ZEN_TASK_SCHED          ZEN_SPT_TASK_SCHED
#else                           // slightly-revamped FreeBSD ULE for the desktop
#define ZEN_TASK_SCHED          ZEN_ULE_TASK_SCHED
#endif
#define ZEN_ULE_TASK_SCHED      1
#define ZEN_BVT_TASK_SCHED      2
#define ZEN_SPT_TASK_SCHED      3
#define ZEN_IP4_NET             1
#if defined(ZEN_MULTIPROC)
#define ZEN_SMP                 1
#endif
#define ZEN_MAX_CPUS            32
#define ZEN_MAX_TASKS           256

/* system limits */
#define ZEN_NAME_MAX            255
#define ZEN_PATH_MAX            4096
#define ZEN_OPEN_MAX            1024
#define ZEN_STREAM_MAX          1024
#define ZEN_SHM_MAX             1024
#define ZEN_EVQ_MAX             (MACH_PAGE_SIZE / ZEN_MAX_EV_SIZE)
#define ZEN_HOST_NAME_MAX       15
#define ZEN_LOGIN_NAME_MAX      15

/* system parameters */
#define ZEN_USR_STK_SIZE        (4 * MACH_PAGE_SIZE)
#define ZEN_USR_STK_MAX         (32 * MACH_PAGE_SIZE)
#define ZEN_SYS_STK_SIZE        MACH_PAGE_SIZE

#endif /* ZEN_CONF_H */

