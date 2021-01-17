#ifndef SYS_ZEN_SCHED_H
#define SYS_ZEN_SCHED_H

/* system threads */
#define ZEN_KERNEL_TASK         0       // kernel
#define ZEN_INIT_TASK           1       // init
#define ZEN_PAGE_TASK           2       // page-daemon
#define ZEN_BUF_TASK            3       // buf-daemon
#define ZEN_SYS_TASKS           4       // # of predefined system threads
#define ZEN_USR_PID_MIN         ZEN_SYS_TASKS
#define ZEN_USR_PID_MAX         (ZEN_MAX_TASKS - 1)

#endif /* SYS_ZEN_SCHED_H */

