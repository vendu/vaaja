#include <stddef.h>
#include <mach/asm.h>
#include <mach/types.h>
#include <mt/mtx.h>
#include <sys/zen/util.h>
#include <sys/zen/mem.h>

#define TABHASH_SLOTS       16384
#define TABHASH_TAB_ITEMS   30
#define TABHASH_ITEM_T      struct zenmemitem
#define TABHASH_CMP(i, k)   ((i)->key == (k))
/* key = adr, val = slab */
#define TABHASH_CHK(i)      (((i)->key && ((i)->val)))
#include <zen/tabhash.h>

struct tabhashtab          *TABHASH_TAB[TABHASH_SLOTS];
volatile struct tabhashtab *TABHASH_BUF;

#if 0
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
#endif

#define zenaddslab(adr, val)    tabhashadd((const uintptr_t)adr,        \
                                           (const uintptr_t)val)
#define zenfindslab(adr)        tabhashop((const uintptr_t)adr, TABHASH_FIND)

#if !defined(SMP)
#define zenchkslabbit(slab, ndx, id)                                    \
    (slab->bmap[ndx] & (1L << (id)))
#else
#define zenchkslabbit(slab, ndx, id)\
    m_cmpclrbit(&slab->bmap[ndx], id)
#endif

static void
zenfreememslab(void *adr)
{
    struct tabhashitem  item = zenfindslab(adr);
    struct zenmemslab  *slab = item.val
                               ? (void *)item.val
                               : NULL;
    struct zenmemqueue *queue;
    uintptr_t           base;
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
        val = 1 << id;
        base = slab->base;
        id = ((uintptr_t)adr - base) >> val;
        ndx = val & ((1 << MACH_LONG_SIZE_LOG2) - 1);
        id = val >> MACH_LONG_SIZE_LOG2;
        if (zenchkslabbit(slab, ndx, id)) {
#if !defined(SMP)
            m_clrbit((m_atomic_t *)&slab->bmap[ndx], id);
            slab->nref--;
            if (!slab->nref) {
                val = queue->slot;
                queue->free(adr);
            }
#else
            /* allocation bitmap update/free successful */
            n = m_fetchadd(&slab->nref, -1);
            if (n == 1) {
                /* slab free */
                mtlkfmtx(&slab->mtx);
                if (!slab->nref) {
                    /* slab still free */
                    val = queue->slot;
                    queue->free(adr);
                } else {
                    mtunlkfmtx(&slab->mtx);
                }
            }
#endif
        } else {
            kdebug("PANIC: duplicate free\n", adr);
        }
    } else {
        /* ZEN_MEM_RUN */
        id = queue->slot;
        val = MACH_PAGE_SIZE * id;
        slab = zenmemslab(adr, val, id);
    }

    return;
}

static void
zeninitmemqueue(struct zenmemqueue *queue, m_word_t type, m_word_t slot)
{
    //    queue->mark = 0;
    queue->type = type;
    queue->slot = slot;
    queue->nitem = 0;
    queue->nref = 0;
    //    queue->head = (m_adr_t)&queue->mark;
    //    queue->tail = (m_adr_t)&queue->mark;
#if 0
    if (type == ZEN_MEM_BLK) {
        queue->free = zenfreememblk;
    } else {
        queue->free = zenfreememrun;
    }
#endif
    queue->free = zenfreememslab;

    return;
}

