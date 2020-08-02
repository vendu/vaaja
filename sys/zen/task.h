#ifndef __SYS_ZEN_TASK_H__
#define __SYS_ZEN_TASK_H__

#define THR_STK             (1L << 0)   // stk + stklim attributes present
#define THR_JOINABLE        (1L << 1)   // thr may be joined
#define THR_SYS_SCOPE       (1L << 2)   // thr has system scope (process)
#define THR_SCHED_OTHER     (1L << 3)   // default scheduler policy
#define THR_SCHED_DEADLINE  (1L << 4)   // earliest deadline first
#define THR_SCHED_FIFO      (1L << 5)   // realtime within a certain timeslice
#define THR_SCHED_RR        (1L << 6)   // realtime round-robin, yield processor
#define THR_SCHED_BATCH     (1L << 7)   // batch scheduler policy
#define THR_SCHED_IDLE      (1L << 8)   // idle scheduler policy
#define THR_STK             (1L << 28)
#define THR_JOINABLE        (1L << 29)
#define THR_SYS_SCOPE       (1L << 30)
#define THR_INHERIT_SCHED   (1L << 31)

#define THR_DEF_FLG THR_STK | THR_JOINABLE | THR_SYS_SCOPE | THR_INHERIT_SCHED

struct thratr {
    m_adr_t     pret;   // thread return/exit value pointer
    m_adr_t     stk;    // thread stack top
    m_size_t    stklim; // thread stack size limit
    m_word_t    flg;    // thread flags
};

#endif /* __SYS_ZEN_TASK_H__ */
