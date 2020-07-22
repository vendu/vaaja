#include <stddef.h>
#include <mach/asm.h>
#include <mach/types.h>
#include <sys/zen/util.h>
#include <sys/zen/mem.h>

#define TABHASH_TAB_ITEMS   30
#define TABHASH_ITEM_T      struct zenmemitem
#define TABHASH_INVALID     { 0, NULL }
#define TABHASH_CMP(i, v)   ((i)->adr == (val))
#define TABHASH_CHK(i)      (((i)->adr) && ((i)->slab))
struct zenmemitem {
    uintptr_t           adr;
    struct zenmemslab  *slab;
};
#include <zen/tabhash.h>

static void
zenfreememblk(void *ptr)
{
    return;
}

static void
zenfreememrun(void *ptr)
{
    return;
}

static void
zeninitmemqueue(struct zenmemqueue *queue, m_word_t type, m_word_t slot)
{
    queue->mark = 0;
    queue->type = type;
    queue->slot = slot;
    queue->nitem = 0;
    queue->nref = 0;
    queue->head = &queue->mark;
    queue->tail = &queue->mark;
    if (type == ZEN_MEM_BLK) {
        queue->free = zenfreememblk;
    } else {
        queue->free = zenfreememrun;
    }

    return;
}

static void
zenfreemem(void *adr)
{
    struct zenmemslab  *slab = zenmemslab(adr);
    struct zenmemqueue *queue;
    m_byte_t           *base;
    m_word_t            val;
    m_word_t            ndx;
    m_word_t            id;
    long                n;

    if (slab) {
        val = slab->type;
        queue = slab->queue;
    }
    if (!queue) {
        /* ZEN_MEM_BIG */
        unmapanon(adr, slab->size);
    } else if (val == ZEN_MEM_BLK) {
        id = queue->slot;
        slab = zenmemslab(adr, val, id);
        val = 1 << id;
        base = slab->base;
        id = (uintptr_t)((m_byte_t *)adr - base) >> val;
        ndx = val & ((1 << MACH_LONG_SIZE_LOG2) - 1);
        id = val >> MACH_LONG_SIZE_LOG2;
        if (m_cmpclrbit(&slab->bmap[ndx], id)) {
            /* allocation bitmap update/free successful */
            n = m_fetchadd(&slab->nref, -1);
            if (n == 1) {
                /* slab free */
                mtlkfmtx(&slab->mtx);
                if (!queue->nref) {
                    /* slab still free */
                    val = queue->slot;
                    queue->free(adr);
                } else {
                    mtunlkfmtx(&slab->mtx);
                }
            }
        } else {
            kdebug("PANIC: duplicate free\n", adr);
        }
    } else {
        /* ZEN_MEM_RUN */
        id = queue->slot;
        slab = zenmemslab(adr, val, id);
        val = MACH_PAGE_SIZE * id;
        base = slab->base;
    }

    return;
}

