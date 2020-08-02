#include <stddef.h>
#include <sys/zen/mem.h>
#include <sys/zen/vm.h>
#include <sys/zen/sched/types.h>
#include <sys/zen/sched/task.h>
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
    m_word_t            qid;

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
    queue->n++;
    mtunlktkt(&queue->lk);

    return;
}

void
zenvmdelpage(struct zenvmpage *page)
{
    struct zenvmqueue  *queue;
    struct zenvmpage   *item;
    m_word_t            qid;

    queue = page->queue;
    item = page->prev;
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
    queue->n--;
    mtunlktkt(&queue->lk);

    return;
}

