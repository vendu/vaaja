#include <stddef.h>
#include <zen/mem.h>
#include <zen/vm.h>
#include <mt/lk.h>

static struct zenvmqueue    g_vmpagequeue[ZEN_VM_QUEUES];

void
zenvmqueuepage(struct zenproc *proc, struct zenvmpage *page)
{
    ;
}

void
zenvmdeqpage(struct zenproc *proc)
{
    ;
}

void
zenvmaddpage(struct zenvmpage *page)
{
    struct zenvmqueue  *queue;
    struct zenvmpage   *tail;
    zenlong             qcnt;
    zenlong             qid;

    page->qcnt++;
    qid = _zencalcvmqueue(page);
    queue = &g_vmpagequeue[qid];
    mtlktkt(&queue->lk);
    tail = queue->tail;
    if (tail) {
        tail->next = page;
    } else {
        queue->head = page;
    }
    page->prev = tail;
    page->next = NULL;
    queue->tail = page;
    mtunlktkt(&queue->lk);

    return;
}

void
zenvmdelpage(struct zenvmpage *page)
{
    struct zenvmqueue  *queue;
    struct zenvmpage   *item;
    zenlong             qcnt;
    zenlong             qid;

    queue = page->queue;
    item = page->prev;
    qcnt = page->qcnt;
    qid = _zencalcvmqueue(page);
    queue = &g_vmpagequeue[qid];
    mtlktkt(&queue->lk);
    if (item) {
        item->next = page->next;
    } else {
        queue->head = page->next;
    }
    item = page->next;
    if (item) {
        item->prev = page->prev;
    } else {
        queue->tail = page->prev;
    }
    mtunlktkt(&queue->lk);

    return;
}

