#include <stddef.h>

static struct mtlfq     g_vmpagequeue[ZEN_VM_QUEUES];

void
zenvmqueuepage(struct zenproc *proc, struct zenvmpage *page)
{
    struct zenvmpage    *next;
    uintptr_t            hptr;

    _zenproclkpageq(proc);
    next = page->next;
    hptr = (uintptr_t)proc->pageq;
    if (next) {
        next->prev = page;
    }
    page->next = (struct zenvmpage *)(hptr & ~MT_MEM_LK_BIT);
    m_cmpswapptr((m_atomicptr_t *)&proc->pageq,
                 (m_atomicptr_t)hptr,
                 (m_atomicptr_t)page);
    zenvmaddpage(page);

    return;
}

void
zenvmdeqpage(struct zenproc *proc)
{
    struct zenvmpage *head;
    struct zenvmpage *next;
    uintptr_t         hptr;

    _zenproclkpageq(proc);
    hptr = (uintptr_t)proc->pageq;
    head = (struct zenvmpage *)(hptr & ~MT_MEM_LK_BIT);
    if (head) {
        next = head->next;
        m_atomwrite(&proc->pageq, next);
        zenvmdelpage(head);
    } else {
        m_atomwrite(&proc->pageq, NULL);
    }

    return;
}

void
zenvmaddpage(struct zenvmpage *page)
{
    zenlong             qcnt;
    zenlong             qofs;
    struct zenvmqueue  *queue;
    struct zenvmpage   *head;
    struct zenvmpage   *tail;

    qcnt = m_fetchadd(&page->qcnt, 1);
    zenvmcalcqid(qcnt, qid);
    queue = &g_vmpagequeue[qid];
    qofs = mtlfqadd(queue, page);
    page->qofs = ofs;
}

void
zenvmdelpage(struct zenvmpage *page)
{
    struct zenvmqueue *queue;
    long               ofs;

    queue = page->queue;
    ofs = page->qofs;
    mtlfqdel(queue, ofs, page);

    return;
}

