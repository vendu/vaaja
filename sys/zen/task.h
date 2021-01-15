#ifndef SYS_ZEN_TASK_H
#define SYS_ZEN_TASK_H

#define ZEN_THR_STK             (1L << 0)   // stk + stklim attributes present
#define ZEN_THR_JOINABLE        (1L << 1)   // thr may be joined
#define ZEN_THR_SYS_SCOPE       (1L << 2)   // thr has system scope (process)
#define ZEN_THR_SCHED_OTHER     (1L << 3)   // default scheduler policy
#define ZEN_THR_SCHED_DEADLINE  (1L << 4)   // earliest deadline first
#define ZEN_THR_SCHED_FIFO      (1L << 5)   // realtime within a certain timeslice
#define ZEN_THR_SCHED_RR        (1L << 6)   // realtime round-robin, yield processor
#define ZEN_THR_SCHED_BATCH     (1L << 7)   // batch scheduler policy
#define ZEN_THR_SCHED_IDLE      (1L << 8)   // idle scheduler policy
#define ZEN_THR_INHERIT_SCHED   (1L << 31)

#define ZEN_THR_DEF_FLG         (ZEN_THR_STK | ZEN_THR_JOINABLE | ZEN_THR_SYS_SCOPE | ZEN_THR_INHERIT_SCHED)

struct thratr {
    m_adr_t                     retptr; // thread return/exit value pointer
    m_adr_t                     stk;    // thread stack top
    m_size_t                    stklim; // thread stack size limit
    m_word_t                    flags;  // thread flags
};

#endif /* SYS_ZEN_TASK_H */

