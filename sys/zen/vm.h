#ifndef __ZEN_VM_H__
#define __ZEN_VM_H__

/* zen kernel virtual memory interface */
#if defined(__v0__)
//#include <zen/bsp/v0.h>
#endif
#include <zen/types.h>
#include <zero/trix.h>
#include <mach/param.h>
#include <mt/tktlk.h>

#if (MACH_WORD_SIZE == 4)
#define zenvmcalcqid    lzero32
#define ZEN_VM_QUEUES   32
#elif (MACH_WORD_SIZE == 8)
#define zenvmcalcqid    lzero64
#define ZEN_VM_QUEUES   64
#endif

#define _zencalcvmqueue(page)   ceilpow2l(page->qcnt)
#define _zenvmlkqueue(queue)    mtlktkt(&queue->lk)
#define _zenvmunlkqueue(queue)  mtunlktkt(&queue->lk)
struct zenvmqueue {
    mttktlk             lk;
    m_atomic_t          n;
    struct zenvmpage   *head;
    struct zenvmpage   *tail;
};

struct zenvmpage {
    struct zenvmqueue          *queue;
    struct zenvmpage           *prev;
    struct zenvmpage           *next;
    int8_t                     *ptr;
    zenlong                     flg;
    volatile m_atomic_t         qcnt;
    zenlong                     qofs;
};

struct zenvmseg {
    const char         *name;
    int8_t             *ptr;
    size_t              size;
    zenlong             flag;
};

#endif /* __ZEN_VM_H__ */

