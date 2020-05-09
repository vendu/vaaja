#ifndef __MT_WAITQ_H__
#define __MT_WAITQ_H__

/* REFERENCE: https://arxiv.org/pdf/1112.1141.pdf */

#include <stdlib.h>
#include <zero/cdefs.h>
#include <mt/mtx.h>
#include <mt/cond.h>
#if !defined(COND_T)
#define COND_T zerocond
#endif

#define WAITQ_MAX         (1L << 16)
#define WAITQ_NONE        (~0L)
#define WAITQ_EMPTY       ((void *)WAITQ_NONE)

struct waitqitem {
    struct waitq     *wq;
    struct waitqitem *next;
};

#define WAITQ_STATUS_NONE (~0L)
#define WAITQ_SIGNAL_BIT  0x01L
#define WAITQ_WAITER_BIT  0x02L
struct waitq {
#if (ZEROFMTX)
    zerofmtx   lk;      // mutual exclusion
#endif
    void      *data;
    long       id;      // waitq ID
    long       flg;     // flag-bits
    long       status;  // application status
    long       event;   // event type
    long       prev;    // previous on queue
    long       next;    // next on queue
    COND_T     cond;    // condition variable
    void     (*signal)(void);
    void     (*wait)(void);
    long     (*poll)(void);
};

struct waitq * waitqgetnext(long waitq);
struct waitq * waitqget(void);
long           waitqgetsize(struct waitq *waitq);

#endif /* __MT_WAITQ_H__ */

