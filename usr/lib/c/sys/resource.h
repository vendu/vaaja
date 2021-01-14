#ifndef __SYS_RESOURCE_H__
#define __SYS_RESOURCE_H__

#include <features.h>
//#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

#define RUSAGE_SELF             0
#define RUSAGE_CHILDREN         1
#if defined(_GNU_SOURCE)
#define RUSAGE_THREAD           2
#endif

/* which argument for set/getpriority() */
#define PRIO_PROCESS            1
#define PRIO_PGRP               2
#define PRIO_USER               3

/* resource limit special values */
#define RLIM_INFINITY           0ULL /* no limit */
#define RLIM_SAVED_MAX          0ULL
#define RLIM_SAVED_CUR          0ULL

/* which argument for set/getrlimit() */
#define RLIMIT_CORE             0 // size of core file in bytes
#define RLIMIT_CPU              1 // CPU time per process in seconds (SIGXCPU)
#define RLIMIT_DATA             2 // data segment size in bytes
#define RLIMIT_FSIZE            3 // file size in bytes
#define RLIMIT_NOFILE           4 // # of open files
#define RLIMIT_STACK            5 // stack size in bytes
#define RLIMIT_AS               6 // address space size

typedef unsigned long long      rlim_t;

struct rlimit {
    rlim_t                      rlim_cur; // soft limit
    rlim_t                      rlim_max; // hard limit
};

struct rusage {
    struct timeval              ru_utime;    /* user CPU time used */
    struct timeval              ru_stime;    /* system CPU time used */
    long                        ru_maxrss;   /* maximum resident set size */
    long                        ru_ixrss;    /* integral shared memory size */
    long                        ru_idrss;    /* integral unshared data size */
    long                        ru_isrss;    /* integral unshared stack size */
    long                        ru_minflt;   /* page reclaims (soft page faults) */
    long                        ru_majflt;   /* page faults (hard page faults) */
    long                        ru_nswap;    /* swaps */
    long                        ru_inblock;  /* block input operations */
    long                        ru_oublock;  /* block output operations */
    long                        ru_msgsnd;   /* IPC messages sent */
    long                        ru_msgrcv;   /* IPC messages received */
    long                        ru_nsignals; /* signals received */
    long                        ru_nvcsw;    /* voluntary context switches */
    long                        ru_nivcsw;   /* involuntary context switches */
};

#if !defined(__zen__)

/* resource usage statistics */
extern int getrusage(int who, struct rusage *rusage);
/* nice value routines */
extern int getpriority(int which, id_t who);
extern int setpriority(int which, id_t who, int prio);
/* resource limit routines */
extern int getrlimit(int which, struct rlimit *lim);
extern int setrlimit(int which, const struct rlimit *lim);

#endif /* !defined(__zen__) */

#endif /* __SYS_RESOURCE_H__ */

