#ifndef SYS_TAIKA_CONF_H
#define SYS_TAIKA_CONF_H

#define TAIKA_TIME_BITS         32
#define TAIKA_VFS_OFF_BITS      32

/* Taika kernel compile-time configuration; deck configuration */

#define TAIKA_GFX_DECK          1
#define TAIKA_TTY               1
#define TAIKA_VT                1
#define TAIKA_PIX_FORMAT        GFX_RGB444

/*
 * in simulation mode, th kernel runs on top of a rurnning system instead of
 * being standalone
 */
#define TAIKA_SIMULATION        1
#define TAIKA_DEBUG             1   // runtime debugging facilities
#if defined(TAIKA_SIMULATION)
#define TAIKA_RUN_ON_UX         1   // execute on top of a Unix-like
#define TAIKA_USE_PTHREAD       1   // virtual kernel with POSIX Threads
#define TAIKA_USE_STDIO         1   // use standard C library I/O (simulation)
#endif /* defined(TAIKA_SIMULATION) */

/* compile-time options */
#define TAIKA_HANG_ON_PANIC     1   // the kerel shall spin on panic
#define TAIKA_TASK_SCHED        TAIKA_UNI_TASK_SCHED
#define TAIKA_ULE_TASK_SCHED    1
#define TAIKA_BVT_TASK_SCHED    2
#define TAIKA_SPT_TASK_SCHED    3
#define TAIKA_UNI_TASK_SCHED    4

#define TAIKA_IP4_NET           1
#if defined(TAIKA_MULTIPROC)
#define TAIKA_SMP               1
#endif
#define TAIKA_MAX_PROCESSORS    32
#define TAIKA_MAX_TASKS         256	// systme max # of tasks

/* system limits */
#define TAIKA_NAME_MAX          255
#define TAIKA_PATH_MAX          4096
#define TAIKA_OPEN_MAX          256
#define TAIKA_STREAM_MAX        256
#define TAIKA_EVQ_SIZE          (2 * MACH_PAGE_SIZE)
#define TAIKA_HOST_NAME_MAX     15
#define TAIKA_LOGIN_NAME_MAX    15
#if 0
#define TAIKA_SHM_MAX           1024
#endif

/* system parameters */
#define TAIKA_USR_STK_SIZE      (4 * MACH_PAGE_SIZE)
#define TAIKA_USR_STK_MAX       (32 * MACH_PAGE_SIZE)
#define TAIKA_SYS_STK_SIZE      MACH_PAGE_SIZE
#define TAIKA_SYS_STK_MAX       (4 * MACH_PAGE_SIZE)

#endif /* SYS_TAIKA_CONF_H */

