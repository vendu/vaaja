#ifndef __ZEN_VM_H__
#define __ZEN_VM_H__

/* zen kernel virtual memory interface */
#if defined(__v0__)
#include <zen/sys/v0.h>
#endif
#include <zero/trix.h>

#if (ZEN_WORD_SIZE == 4)
#define zenvmcalcqid    lzero32
#define ZEN_VM_QUEUES   32
#elif (ZEN_WORD_SIZE == 8)
#define zenvmcalcqid    lzero64
#define ZEN_VM_QUEUES   64
#endif

#define v0calcvmqueue(page)                                             \
#define _zenvmlkqueue(queue)    mtlktkt(&queue->lk)
#define _zenvmunlkqueue(queue)  mtunlktkt(&queue->lk)
struct zenvmqueue {
    mttktlk             lk;
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

