#ifndef SYS_ZEN_TASK_H
#define SYS_ZEN_TASK_H

#include <mt/thr.h>

#define ZEN_TASK_THREAD         (1L << 0)   // task is a thread (if 0, process)
#define ZEN_TASK_STK            (1L << 1)   // stk + stklim attributes present
#define ZEN_TASK_JOINABLE       (1L << 2)   // thr may be joined
#define ZEN_TASK_SYS_SCOPE      (1L << 3)   // thr has system scope (process)
#define ZEN_TASK_SCHED_OTHER    (1L << 4)   // default scheduler policy
#define ZEN_TASK_SCHED_DEADLINE (1L << 5)   // earliest deadline first
#define ZEN_TASK_SCHED_FIFO     (1L << 6)   // realtime within a certain timeslice
#define ZEN_TASK_SCHED_RR       (1L << 7)   // realtime round-robin, yield processor
#define ZEN_TASK_SCHED_BATCH    (1L << 8)   // batch scheduler policy
#define ZEN_TASK_SCHED_IDLE     (1L << 9)   // idle scheduler policy
#define ZEN_TASK_INHERIT_SCHED  (1L << 31)

#define ZEN_TASK_DEF_FLG        (ZEN_TASK_STK                           \
                                 | ZEN_TASK_JOINABLE                    \
                                 | ZEN_TASK_SYS_SCOPE                   \
                                 | ZEN_TASK_INHERIT_SCHED)

struct thratr {
    m_ptr_t                     retptr; // thread return/exit value pointer
    m_word_t                    nice;
    struct uthratr              uatr;
};

#endif /* SYS_ZEN_TASK_H */

