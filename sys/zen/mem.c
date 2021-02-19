#include <sys/zen/conf.h>
#include <stddef.h>
#include <mach/asm.h>
#include <mach/types.h>
#include <mt/mtx.h>
#include <env/cdefs.h>
#include <prng/randmt32.h>
#include <sys/zen/bitbang.h>
#include <sys/zen/util.h>
#include <sys/zen/mem.h>

#if 0
#define TABHASH_SLOTS                   16384
#define TABHASH_TAB_ITEMS               30
#define TABHASH_ITEM_T                  struct zenmemitem
#define TABHASH_CMP(i, k)               ((i)->key == (k))
/* key = adr, val = slab */
#define TABHASH_CHK(i)                  (((i)->key && ((i)->val)))
#include <zen/tabhash.h>

static struct tabhashtab               *TABHASH_TAB[TABHASH_SLOTS];
static volatile struct tabhashtab      *TABHASH_BUF;
#endif /* 0 */

static int32_t                          memprimetab[32]
= {
    1, 2, 3, 5, 7, 11, 13, 17,
    19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83,
    89, 97, 101, 103, 107, 109, 113, 127
};

#if defined(ZEN_SIMULATION)
#include <stdlib.h>
#define kmapanon(size, flg)             mapanon(-1, size, flg)
#define kmalloc(size)                   malloc(size)
#define kfree(ptr)                      free(ptr)
#define kmemalign(align, size)          aligned_alloc(align, size)
#define krealloc(ptr, size)             realloc(ptr, size)

#else /* !defined(ZEN_SIMULATION) */

static C_INLINE void *
kmalloc(m_size_t size)
{
    return NULL;
}

static C_INLINE void
kfree(void *ptr)
{
    return NULL;
}

static C_INLINE void *
kmemalign(size_t align, size_t size)
{
    void                       *ptr;

    ptr = kgetmem(align, size);
}

static C_INLINE void *
krealloc(void *ptr, size_t size)
{
    void                       *ret;

    ret = kmalloc(size);
    if ((ret) && (ptr)) {
        kmemcpy(ret, ptr, size);
        ptr = NULL;
    }

    return ret;
}

#endif /* defined(ZEN_SIMULATION) */

#if 0
static void
zenfreeblk(void *ptr)
{
    return;
}

static void
zenfreerun(void *ptr)
{
    return;
}
#endif

#if 0
#define zenaddadr(adr, val)    tabhashadd((const uintptr_t)adr,         \
                                          (const uintptr_t)val)
#define zenfindadr(adr)        tabhashop((const uintptr_t)adr, TABHASH_FIND)
#endif

#define zengetslabbit(slab, ndx, id)                                    \
    m_cmpclrbit(&slab->bmap[ndx], id)

struct zenmemslab              *memslabtab[ZEN_MEM_MAX_SLABS] C_ALINED(MACH_PAGE_SIZE);

#define zenmemaddblk(adr, buf)  (memslabtab[(uintptr_t)(adr) >> 17] = (buf))
#define zenfindbuf(adr)         (memslabtab[(uintptr_t)(adr) >> 17])
static void
zenfreememslab(void *adr)
{
    struct tabhashitem  item = zenfindslab(adr);
    struct zenmemslab  *slab = item.val
                               ? (void *)item.val
                               : NULL;
    struct zenmempool  *pool;
    uintptr_t           base;
    m_word_t            val;
    m_word_t            ndx;
    m_word_t            id;
    long                n;

    if (slab) {
        val = slab->type;
        pool = slab->pool;
    }
    if (!pool) {
        /* ZEN_MEM_BIG */
        unmapanon(adr, slab->size);
    } else if (val == ZEN_MEM_BLK) {
        id = pool->slot;
        val = 1 << (id + MACH_CL_SIZE_LOG2);
        base = slab->base;
        id = ((uintptr_t)adr - base) >> val;
        ndx = val & ((1 << MACH_LONG_SIZE_LOG2) - 1);
        id = val >> MACH_LONG_SIZE_LOG2;
        if (zenchkslabbit(slab, ndx, id)) {
#if !defined(SMP)
            slab->nfree--;
            if (!slab->nfree) {
                pool->free(adr);
            }
#else
            /* allocation bitmap update/free successful */
            n = m_fetchadd(&slab->nfree, -1);
            if (n == 1) {
                /* slab free */
                mtlkfmtx(&slab->mtx);
                if (!slab->nref) {
                    /* slab still free */
                    pool->free(adr);
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
        id = pool->slot;
        val = 1 << (id + MACH_CL_SIZE_LOG2);
        slab = zenmemslab(adr, val, id);
    }

    return;
}

static void
zeninitmempool(struct zenmempool *pool, m_word_t zone, m_word_t slot)
{
    pool->free = zenfreememslab;
    pool->head = &pool->bmap;
    pool->tail = &pool->bmap;
    pool->nref = 0;
    pool->zone = zone;
    pool->flg = flg;
    pool->slot = slot;
    pool->nitem = 0;

    return;
}

static void
zeninitmem(void)
{
    srandmt32(0xa5a5a5a5);

    return;
}

