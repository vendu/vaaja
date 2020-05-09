#include <stdlib.h>
#include <errno.h>
#include <mt/mtx.h>
#include <zen/sys/unix.h>
#include <zen/mem.h>
#include <zen/bitop.h>

struct tabhashtab              *g_memhashtab[TABHASH_SLOTS];
struct memglob                  g_mem ALIGNED(PAGESIZE);
THREADLOCAL struct memtls      *t_memtls;
struct tabhashtab              *g_memhashbuf;

#define memblkslabsize(qid)                                             \
    (((qid) <= MEM_BLK_MID_QUEUE_MIN)                                   \
     ? (MEM_BLK_MIN_SLAB >> 2)                                          \
     : (((qid) <= MEM_BLK_BIG_QUEUE_MIN)                                \
        ? (MEM_BLK_MID_SLAB >> 1)                                       \
        : MEM_BLK_BIG_SLAB))
static void
meminitblktabs(struct memglob *mem)
{
    double              slabsz;
    double              blksz;
    double              nblk;
    double              multi;
    unsigned long       qid;

    for (qid = 0 ; qid < MEM_BLK_QUEUES ; qid++) {
        slabsz = memblkslabsize(qid);
        blksz = MEM_BLK_MIN * (double)(qid + 1);
        nblk = slabsz / blksz;
        multi = 1.0 / blksz;
        mem->nblktab[qid] = (long)nblk;
        mem->blkdivtab[qid] = multi;
    }

    return;
}

#define memrunslabsize(qid)                                             \
    (((qid) < MEM_RUN_MID_QUEUE_MIN)                                    \
     ? MEM_RUN_MIN_SLAB                                                 \
     : (((qid) < MEM_RUN_BIG_QUEUE_MIN)                                 \
        ? MEM_RUN_MID_SLAB                                              \
        : MEM_RUN_BIG_SLAB))
static void
meminitruntabs(struct memglob *mem)
{
    double              slabsz;
    double              runsz;
    double              nrun;
    double              multi;
    unsigned long       qid;

    for (qid = 0 ; qid < MEM_RUN_QUEUES ; qid++) {
        slabsz = (double)memrunslabsize(qid);
        runsz = PAGESIZE * (double)(qid + 1);
        nrun = slabsz / runsz;
        multi = 1.0 / runsz;
        mem->nruntab[qid] = (long)nrun;
        mem->rundivtab[qid] = multi;
    }

    return;
}

void
meminit(struct memglob *mem)
{
    mtlkfmtx(&mem->mtx);
    if (!(mem->flg & MEM_INIT_BIT)) {
        meminitblktabs(mem);
        meminitruntabs(mem);
    }
    mtunlkfmtx(&mem->mtx);

    return;
}

#define MEM_FIND_PAGE   0
#define MEM_FIND_VAL    1
uintptr_t
memfindbuf(void *ptr, long which, uintptr_t mask)
{
    TABHASH_ITEM_T      item = tabhashfind(g_memhashtab, (uintptr_t)ptr);
    uintptr_t           ret = 0;

    if (item.page) {
        switch (which) {
            case MEM_FIND_PAGE:
                ret = item.page;

                break;
            case MEM_FIND_VAL:
                ret = item.val;
            default:

                break;
        }
    }
    ret &= mask;

    return ret;
}

/* acquire buffer header */
static struct membuf *
memgetbuf(long type)
{
    struct membufq               *queue = NULL;
    volatile struct membuf      **hdrq;
    long                          ntry = 32;
    size_t                        bufsz;
    size_t                        nbuf;
    struct membuf                *ret;
    int8_t                       *ptr;
    struct membuf                *buf;
    struct membuf                *head;
    struct membuf                *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    if (type == MEM_BLK) {
        bufsz = memblkbufsize();
        nbuf = MEM_CACHE_BLK_BUFS;
        hdrq = &g_mem.blkhdrq;
    } else {
        bufsz = membufsize();
        nbuf = MEM_CACHE_BUFS;
        hdrq = &g_mem.bufhdrq;
    }
    do {
        mtlkbit((m_atomic_t *)hdrq, MEM_LK_BIT_OFS);
        ret = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, nbuf * bufsz, 0);
                if (ptr != MAP_FAILED) {
                    buf = (struct membuf *)ptr;
                    ret = (struct membuf *)ptr;
                    buf->queue = queue;
                    head = (struct membuf *)((int8_t *)buf + bufsz);
                    head->queue = queue;
                    head->prev = NULL;
                    prev = head;
                    while (--nbuf) {
                        buf = (struct membuf *)((int8_t *)buf + bufsz);
                        buf->queue = queue;
                        buf->prev = prev;
                        prev->next = buf;
                        prev = buf;
                    }
                    buf->next = NULL;
                }
            }
            if (ptr != MAP_FAILED) {
                ret = (struct membuf *)ptr;
            }
        }
        if (ret) {
            head = ret->next;
            if (head) {
                head->prev = NULL;
            }
        } else {
            head = NULL;
        }
        m_atomwrite((m_atomic_t *)hdrq, (m_atomic_t *)head);
        if (!ret) {
            m_waitspin();
        }
    } while (!ret && --ntry);

    return ret;
}

/* release buffer header */
static void
memputbuf(struct membuf *buf, long type)
{
    volatile struct membuf    **hdrq;
    struct membuf              *head;

    buf->adr = NULL;
    buf->prev = NULL;
    if (type == MEM_BLK) {
        hdrq = &g_mem.blkhdrq;
    } else {
        hdrq = &g_mem.bufhdrq;
    }
    mtlkbit((m_atomic_t *)hdrq, MEM_LK_BIT_OFS);
    head = (void *)((uintptr_t)*hdrq & ~MEM_LK_BIT);
    buf->next = head;
    if (head) {
        head->prev = buf;
    }
    m_atomwrite((m_atomic_t *)hdrq, buf);

    return;
}

/* push buffer in front of queue */
static void
mempushbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf       *head = queue->head;

    buf->prev = NULL;
    buf->next = head;
    queue->nbuf++;
    if (head) {
        head->prev = buf;
    } else {
        queue->tail = buf;
    }
    queue->head = buf;

    return;
}

static void
mempopbuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf       *next;

    next = buf->next;
    if (next) {
        next->prev = NULL;
    } else {
        queue->tail = NULL;
    }
    queue->head = next;

    return;
}

/* queue buffer in back of queue */
static void
memqueuebuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *tail = queue->tail;

    buf->next = NULL;
    buf->prev = tail;
    queue->nbuf++;
    if (tail) {
        tail->next = buf;
    } else {
        queue->head = buf;
    }
    queue->tail = buf;

    return;
}

/* remove buffer from queue */
static void
memdequebuf(struct membufq *queue, struct membuf *buf)
{
    struct membuf      *prev;
    struct membuf      *next;

    prev = buf->prev;
    next = buf->next;
    queue->nbuf--;
    if (prev) {
        prev->next = buf->next;
    } else {
        queue->head = next;
    }
    if (next) {
        next->prev = buf->prev;
    } else {
        queue->tail = prev;
    }

    return;
}

/* allocate new block buffer */
static void *
memgetblk(unsigned long qid, size_t align, size_t *retsize)
{
    int8_t             *ptr = NULL;
    int8_t             *ret = NULL;
    struct membuf      *buf = t_memtls->blktab[qid].head;
    long                ntry = 32;
    size_t              slabsz;
    size_t              blksz = memblksize(qid);
    uintptr_t          *tab;
    long                nblk;
    long                n;
    size_t              ofs;
    size_t              lim;
    size_t              diff;
    uintptr_t           page;
    uintptr_t           val;

    if (!buf) {
        mtlkfmtx(&g_mem.blktab[qid].mtx);
        buf = g_mem.blktab[qid].head;
        if (!buf) {
            slabsz = memblkslabsize(qid);
            mtunlkfmtx(&g_mem.blktab[qid].mtx);
            buf = memgetbuf(MEM_BLK);
            do {
                ptr = mapanon(-1, slabsz, 0);
                if (ptr == MAP_FAILED) {
                    m_waitspin();
                }
            } while (ptr == MAP_FAILED && --ntry);
            if (ptr == MAP_FAILED) {
                memputbuf(buf, MEM_BLK);
                buf = NULL;
            } else {
                /* initialise buffer */
                nblk = memnblk(qid);
                buf->adr = ptr;
                buf->size = slabsz;
                buf->queue = &t_memtls->blktab[qid];
                tab = buf->tab;
                buf->max = nblk;
                /* initialise allocation stack */
                n = nblk >> 3;
                nblk -= 8 * n;
                while (n--) {
                    tab[0] = (uintptr_t)ptr;
                    tab[1] = (uintptr_t)ptr + blksz;
                    tab[2] = (uintptr_t)ptr + 2 * blksz;
                    tab[3] = (uintptr_t)ptr + 3 * blksz;
                    tab[4] = (uintptr_t)ptr + 4 * blksz;
                    tab[5] = (uintptr_t)ptr + 5 * blksz;
                    tab[6] = (uintptr_t)ptr + 6 * blksz;
                    tab[7] = (uintptr_t)ptr + 7 * blksz;
                    ptr += 8 * blksz;
                    tab += 8;
                }
                n = min(8, nblk);
                switch (nblk) {
                    case 8:
                        tab[7] = (uintptr_t)ptr + 7 * blksz;
                    case 7:
                        tab[6] = (uintptr_t)ptr + 6 * blksz;
                    case 6:
                        tab[5] = (uintptr_t)ptr + 5 * blksz;
                    case 5:
                        tab[4] = (uintptr_t)ptr + 4 * blksz;
                    case 4:
                        tab[3] = (uintptr_t)ptr + 3 * blksz;
                    case 3:
                        tab[2] = (uintptr_t)ptr + 2 * blksz;
                    case 2:
                        tab[1] = (uintptr_t)ptr + blksz;
                    case 1:
                        tab[0] = (uintptr_t)ptr;
                    case 0:
                    default:

                        break;
                }
                ptr = (int8_t *)buf->tab[0];
                buf->ofs = 1;
                mempushbuf(&t_memtls->blktab[qid], buf);
            }
        } else {
            ofs = buf->ofs;
            ptr = (int8_t *)buf->tab[ofs];
            lim = buf->max;
            ofs++;
            buf->ofs = ofs;
            if (ofs == lim) {
                mempopbuf(&g_mem.blktab[qid], buf);
            }
            mtunlkfmtx(&g_mem.blktab[qid].mtx);
        }
    } else {
        ofs = buf->ofs;
        ptr = (int8_t *)buf->tab[ofs];
        lim = buf->max;
        ofs++;
        buf->ofs = ofs;
        if (ofs == lim) {
            mempopbuf(&t_memtls->blktab[qid], buf);
        }
    }
    if (ptr) {
        ofs = buf->max;
        ret = memalignptr(ptr, align);
        ofs += memblknum(qid, buf, ret);
        diff = ret - ptr;
        page = memsethashpage(mempageadr(ret), MEM_BLK);
        val = memsethashbuf(buf, qid);
        buf->tab[ofs] = (uintptr_t)diff;
        tabhashadd(g_memhashtab, page, val);
        if (retsize) {
            *retsize = blksz;
        }
    }

    return ret;
}

static void *
memgetrun(unsigned long qid, size_t align, size_t *retsize)
{
    int8_t             *ret;
    int8_t             *ptr = NULL;
    struct membuf      *buf = t_memtls->runtab[qid].head;
    long                ntry = 32;
    size_t              nrun;
    size_t              slabsz;
    size_t              runsz = memrunsize(qid);
    struct membuf      *next;
    long                bits;
    long                mask;
    long                ofs;
    uintptr_t           page;
    uintptr_t           val;
    uintptr_t           diff;

    if (!buf) {
        mtlkfmtx(&g_mem.runtab[qid].mtx);
        buf = g_mem.blktab[qid].head;
        if (!buf) {
            slabsz = memrunslabsize(qid);
            nrun = memnrun(qid);
            do {
                ptr = mapanon(-1, slabsz, 0);
                if (ptr == MAP_FAILED) {
                    m_waitspin();
                }
            } while (ptr == MAP_FAILED && --ntry);
            bits = 1L << nrun;
            if (ptr == MAP_FAILED) {
                memputbuf(buf, MEM_RUN);
                buf = NULL;
            } else {
                /* initialise run buffer */
                bits--;
                buf->adr = ptr;                 // buffer base address
                buf->size = slabsz;             // buffer total size
                buf->bits = bits;               // set free-bitmap
                buf->max = nrun;                // set # of runs in buffer
                mempushbuf(&t_memtls->runtab[qid], buf);
            }
        } else {
            bits = buf->bits;
            ofs = m_ctz(bits);
            mask = 1L << ofs;
            ofs <<= PAGESIZELOG2;
            mask = ~mask;
            ofs *= qid;
            bits &= mask;
            ptr = buf->adr + ofs;
            buf->bits = bits;
            if (!bits) {
                mempopbuf(&g_mem.runtab[qid], buf);
            }
            mtunlkfmtx(&g_mem.runtab[qid].mtx);
        }
    } else {
        bits = buf->bits;
        ofs = m_ctz(bits);
        mask = 1L << ofs;
        ofs <<= PAGESIZELOG2;
        mask = ~mask;
        ofs *= qid;
        bits &= mask;
        ptr = buf->adr + ofs;
        buf->bits = bits;
        if (!bits) {
            mempopbuf(&t_memtls->runtab[qid], buf);
            next = t_memtls->runtab[qid].head;
            if (next) {
                next->prev = NULL;
            } else {
                t_memtls->runtab[qid].tail = NULL;
            }
            t_memtls->runtab[qid].head = next;
        }
    }
    if (ptr) {
        ret = memalignptr(ptr, align);
        ofs = memrunnum(qid, buf, ret);
        diff = ret - ptr;
        page = memsethashpage(mempageadr(ret), MEM_RUN);
        val = memsethashbuf(buf, qid);
        buf->tab[ofs] = (uintptr_t)diff;
        tabhashadd(g_memhashtab, page, val);
        if (retsize) {
            *retsize = runsz;
        }
    }

    return ret;
}

/* release allocation block */
static void
memputblk(struct membuf *buf, void *ptr)
{
    struct membufq     *queue = buf->queue;
    unsigned long       qid = queue->qid;
    struct memtls      *tls = queue->tls;
    long                lim = (tls) ? memnblktlsbuf(qid) : memnblkbuf(qid);
    struct membufq     *glob;
    void               *adr;
    long                ndx;
    long                max;
    long                nbuf;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    adr = memgetptr(ptr);
    ndx = buf->ofs;
    max = buf->max;
    ndx--;
    nbuf = queue->nbuf;
    max--;
    buf->tab[ndx] = (uintptr_t)adr;
    buf->ofs = ndx;
    if (ndx == max) {
        memqueuebuf(queue, buf);
    } else if (!ndx && nbuf > lim) {
        memdequebuf(queue, buf);
        if (!tls) {
            memfreebuf(buf);
        } else {
            glob = &g_mem.blktab[qid];
            nbuf = glob->nbuf;
            lim = memnblkbuf(qid);
            if (nbuf > lim) {
                memfreebuf(buf);
            } else {
                mempushbuf(glob, buf);
            }
        }
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return;
}

/* release allocation run */
static struct membuf *
memputrun(struct membuf *buf, void *adr)
{
    struct membufq     *queue = buf->queue;
    unsigned long       qid = queue->qid;
    struct memtls      *tls = queue->tls;
    long                ndx = memrunnum(qid, buf, adr);
    uintptr_t           bits = buf->bits;
    uintptr_t           mask = (uintptr_t)1 << ndx;
    long                nrun = buf->max;
    long                nbuf = queue->nbuf;
    long                lim = 2;
    struct membufq     *glob;

    if (!tls) {
        mtlkfmtx(&queue->mtx);
    }
    if (bits & mask) {
        /* deal with duplicate free */
        ;
    }
    bits |= mask;
    buf->bits = bits;
    if (bits == mask) {
        memqueuebuf(queue, buf);
    } else {
        mask = (uintptr_t)1 << nrun;
        mask--;
        if (bits == mask && nbuf > lim) {
            memdequebuf(queue, buf);
            if (!tls) {
                memfreebuf(buf);
            } else {
                glob = &g_mem.runtab[qid];
                nbuf = glob->nbuf;
                mask = (uintptr_t)1 << nrun;
                lim = memnrunbuf(qid);
                mask--;
                if (bits == mask && nbuf > lim) {
                    memfreebuf(buf);
                } else {
                    mempushbuf(glob, buf);
                }
            }
        }
    }
    if (!tls) {
        mtunlkfmtx(&queue->mtx);
    }

    return buf;
}

/* acquire big allocation */
void *
memgetbig(size_t size, size_t align, size_t *retsize)
{
    struct membuf      *buf = memgetbuf(MEM_BIG);
    size_t              mapsz = roundup2(size, PAGESIZE);
    int8_t             *ptr;
    int8_t             *ret = NULL;
    size_t              aln;
    size_t              cnt;
    uintptr_t           page;
    uintptr_t           val;

    if (!buf) {

        return NULL;
    }
    ptr = mapanon(-1, mapsz, 0);
    if (ptr == MAP_FAILED) {
        memputbuf(buf, MEM_BIG);

        ptr = NULL;
    } else {
        ret = memalignptr(ptr, align);
        aln = ret - ptr;
        page = memsethashpage(mempageadr(ret), MEM_BIG);
        cnt = m_ctz(aln);
        val = memsethashval(mapsz, cnt);
        tabhashadd(g_memhashtab, page, val);
        mapsz -= aln;
        if (retsize) {
            *retsize = mapsz;
        }
    }

    return ret;
}

/* free (unmap) big allocation */
static void
memfreebig(void *ptr, size_t size)
{
    tabhashdel(g_memhashtab, (uintptr_t)ptr);
    unmapanon(ptr, size);

    return;
}

/* allocate size bytes, align to boundary of align bytes */
void *
memget(size_t size, size_t align, size_t *retsize)
{
    void               *ptr = NULL;
    size_t              aln = max(align, MEM_ALIGN_MIN);
    size_t              alnsz = memalnsize(size, aln);
    uintptr_t           type = membuftype(alnsz);
    size_t              blksz = 0;
    unsigned long       qid;

    if (!(g_mem.flg & MEM_INIT_BIT)) {
        meminit(&g_mem);
    }
    switch (type) {
        case MEM_BLK:
            blksz = roundup2(alnsz, MEM_BLK_MIN);
            qid = memblkqid(blksz);
            ptr = memgetblk(qid, aln, &blksz);

            break;
        case MEM_RUN:
            blksz = roundup2(alnsz, PAGESIZE);
            qid = memrunqid(blksz);
            ptr = memgetrun(qid, aln, &blksz);

            break;
        case MEM_BIG:
            blksz = roundup2(alnsz, PAGESIZE);
            ptr = memgetbig(blksz, aln, &blksz);
        default:

            break;
    }
    if ((ptr) && (retsize)) {
        *retsize = blksz;
    }

    return ptr;
}

#define memsize(ptr) 0 /* FIXME */
void *
memresize(void *ptr, size_t size, size_t align, long flg)
{
    size_t      alnsz = memalnsize(size, align);
    size_t      srcsz = (ptr) ? memsize(ptr) : 0;
    void       *adr = (ptr) && srcsz >= alnsz ? ptr : NULL;
    size_t      cpysz = 0;
    size_t      destsz;

    if (!adr) {
        if (ptr) {
            adr = memget(size, align, &destsz);
        }

        return NULL;
    }
#if 0
    if ((!ptr && (flg & MEM_ALLOC_ON_NULL))
        || !adr) {
        adr = memget(size, align, &destsz);
    } else {
        adr = ptr;
    }
#endif
    if (adr) {
        if ((ptr) && adr != ptr) {
            cpysz = min(srcsz, destsz);
            memcpy(adr, ptr, cpysz);
        }
    }
    if (!adr) {
        errno = ENOMEM;
        if (flg & MEM_FREE_ON_FAILURE) {
            memput(ptr);
        }
    }

    return adr;
}

/* release allocation */
void
memput(void *ptr)
{
    uintptr_t           page = mempageadr(ptr);
    struct memhash      hash = tabhashop(g_memhashtab, page, TABHASH_REMOVE);

    if (!(g_mem.flg & MEM_INIT_BIT)) {
        abort();
    }
    if (hash.page) {
        struct membuf  *buf = memhashbuf(&hash);
        long            type = memhashtype(&hash);
        size_t          size = memhashsize(&hash);

        switch (type) {
            case MEM_BLK:
                memputblk(buf, ptr);

                break;
            case MEM_RUN:
                memputrun(buf, ptr);

                break;
            case MEM_BIG:
                memfreebig(buf->adr, size);
            default:

                break;
        }
    }

    return;
}

