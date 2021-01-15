#ifndef SYS_ZEN_SCHED_H
#define SYS_ZEN_SCHED_H

/* system threads */
#define ZEN_KERNEL_THREAD       0       // kernel
#define ZEN_INIT_THREAD         1       // init
#define ZEN_PAGE_THREAD         2       // page-daemon
#define ZEN_BUF_THREAD          3       // buf-daemon
#define ZEN_SYS_THREADS         4       // # of predefined system threads
#define ZEN_USR_PID_MIN         ZEN_SYS_THREADS
#define ZEN_USR_PID_MAX         (ZEN_MAX_TASKS - 1)

#endif /* SYS_ZEN_SCHED_H */

