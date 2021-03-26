#include <stddef.h>
#include <stdint.h>
#include <mt/mtx.h>
#include <kern/util.h>
#include <kern/printf.h>
#include <kern/malloc.h>
#include <kern/mem/vm.h>
#include <kern/mem/membuf.h>

//static struct membufpool membufpooltab[NCPU] ALIGNED(PAGESIZE);
static struct membufpool k_membufpool;

long
membufinit(void)
{
    struct membufpool *tab = &k_membufpool;
    struct membuf     *buf;
    struct memblk     *blk;
    void              *last;
    uint8_t           *u8ptr1;
    uint8_t           *u8ptr2;
    intmax_t           n;

    /* allocate wired memory for membufs */
    u8ptr1 = kwalloc(MEMBUFBLKS * MEMBUF_SIZE);
    if (!u8ptr1) {
        kprintf("FAILED to allocate membufs\n");

        return 0;
    }
    kbzero(u8ptr1, MEMBUFBLKS * MEMBUF_SIZE);
    /* initialise global membuf container */
    n = MEMBUFBLKS;
    tab->nbuf = n;
    u8ptr1 += MEMBUFBLKS * MEMBUF_SIZE;
    last = NULL;
    while (n--) {
        u8ptr1 -= MEMBUF_SIZE;
        buf = (struct membuf *)u8ptr1;
        buf->hdr.next = last;
        last = buf;
    }
    tab->buflist = last;
#if 0
    /* initialise per-CPU buf containers */
    n = NCPU;
    while (n--) {
        meminitcpubuf(n, MEM_DONTWAIT);
    }
#endif

    return 1;
}

/* FIXME: steal membufs from other CPUs if need arises
 * - do lock-free pop of buffer from front of list
 */
struct membuf *
memgetbuf(long how)
{
    struct membufpool *tab = &k_membufpool;
    struct membuf     *ret = NULL;
    uint8_t            *ptr;
    struct membuf     *buf;
    struct membuf     *last;
    long               loop;
    m_ureg_t           n;

    loop = how & MEM_WAIT;
    do {
        fmtxlk(&tab->lk);
        ret = tab->buflist;
        if (ret) {
            tab->buflist = ret->hdr.next;
            tab->nbuf--;
        }
        fmtxunlk(&tab->lk);
    } while (!ret && (loop));

    return ret;
}

void
memputbuf(struct membuf *buf)
{
    struct membufpool *tab = &k_membufpool;

    fmtxlk(&tab->lk);
    buf->hdr.next = tab->buflist;
    tab->buflist = buf;
    tab->nbuf++;
    fmtxunlk(&tab->lk);

    return;
}

#if 0

/* FIXME: steal mbufs from other CPUs if need arises */
void *
memgetcpubuf(long how)
{
    volatile long      unit = k_curunit;
    struct membufpool *tab = &k_membufpooltab[unit];
    uint8_t           *ptr;
    struct memblk     *ret = NULL;
    struct memblk     *blk;
    struct memblk     *last;
    long               loop;
    m_ureg_t           n;

    loop = how & MEM_WAIT;
    do {
        fmtxlk(&tab->lk);
        ret = tab->buflist;
        if (ret) {
            tab->buflist = blk->next;
            tab->nblk--;
        } else {
            fmtxlk(&tab->lk);
            ret = tab->buflist;
            if (ret) {
                tab->buflist = blk->next;
                tab->nblk--;
            }
            fmtxunlk(&tab->lk);
        }
        fmtxunlk(&tab->lk);
    } while (!ptr && (loop));

    return ret;
}

void
memputcpubuf(struct memblk *blk)
{
    long                unit = k_curunit;
    struct membufpool *tab = &k_membufpooltab[unit];

    fmtxlk(&tab->lk);
    blk->next = tab->buflist;
    tab->buflist = blk;
    tab->nblk++;
    fmtxunlk(&tab->lk);
}

/*
 * called without locks at boot time, or with locks held by memgetbuf() */
long
meminitcpubuf(long unit, long how)
{
    struct membufpool *tab = &k_membufpooltab[unit];
    uint8_t           *u8ptr = kwalloc(PAGESIZE);
    m_ureg_t           n = PAGESIZE / MEMBUF_SIZE;
    void              *last = NULL;
    struct membuf     *buf;

    if (!u8ptr) {

        return 0;
    }
    u8ptr += PAGESIZE;
    tab->nbuf = n;
    while (n--) {
        u8ptr -= MEMBUF_SIZE;
        buf = (struct membuf *)u8ptr;
        buf->hdr.next = last;
        last = buf;
    }
    tab->buflist = last;

    return 1;
}

#endif /* 0 */

/* allocate block and refer it to buffer */
static __inline__ void *
memallocext(struct membuf *buf, long how)
{
    void             *ptr;
    struct memexthdr *ext;

    ptr = memalloc(MEMBUF_BLK_SIZE, how);
    if (ptr) {
        ext = &membufexthdr(buf);
        buf->hdr.flg |= MEMBUF_EXT_BIT;
        ext->data = ptr;
        ext->size = MEMBUF_BLK_SIZE;
        ext->type = MEMBUF_EXT_BLK;
    }

    return ptr;
}

/* set up preallocated external storage and refer it to buffer */
static __inline__ void
memsetext(struct membuf *buf, void *adr, m_ureg_t size,
          void (*rel)(void *, void *),
          void *args, long type, long flg)
{
    struct memexthdr *ext;

    ext = &membufexthdr(buf);
    buf->hdr.flg |= MEMBUF_EXT_BIT | flg;
    ext->data = adr;
    ext->size = size;
    ext->rel = rel;
    ext->args = args;
    ext->type = type;

    return;
}

/* release external storage if reference count is zero */
static __inline__ void
memrelext(struct membuf *buf)
{
    struct memexthdr *ext;

    ext = &membufexthdr(buf);
    membufdecref(ext);
    if (m_cmpswap(&ext->nref, 0, 1)) {
        if (ext->type != MEMBUF_EXT_BLK) {
            ext->rel(ext->data, ext->args);
        } else {
            kfree(ext->data);
            kfree(ext->args);
        }
    }
    buf->hdr.flg &= ~MEMBUF_EXT_BIT;
}

/*
 * release buffer and possible associated external storage
 * - return next buffer in chain
 */
static __inline__ struct membuf *
memrelbuf(struct membuf *buf)
{
    struct membuf *next;

    next = membufnext(buf);
    if (buf->hdr.flg & MEMBUF_EXT_BIT) {
        memrelext(buf);
    }
    memputbuf(buf);

    return next;
}

/* free chain of buffers */
static __inline__ void
memrelchain(struct membuf *buf)
{
    struct membuf    *aux;
    struct mempkthdr *pkt;

    if (buf) {
        do {
            if (membufflg(buf) & MEMBUF_PKTHDR_BIT) {
                pkt = &membufpkthdr(buf);
                aux = pkt->aux;
                memrelbuf(aux);
                pkt->aux = NULL;
            }
            buf = memrelbuf(buf);
        } while (buf);
    }

    return;
}

/*
 * allocate len bytes's worth of buffers
 * - return pointer to top of chain;
 *   if buf is non-NULL, allocate chain and return buf
 * - in case of failure we free everything we allocated and return NULL
 * - TODO: implement MEM_BLOCK
 */
static __inline__ struct membuf *
memgetchain(m_ureg_t len, long how, long type)
{
    struct membuf *buf;
    struct membuf *last = NULL;
    struct membuf *top;
    struct membuf *tail;

    buf = memgetbuf(how);
    if (!buf) {

        return NULL;
    } else if (len > MEMBUF_PKT_LEN) {
        memallocext(buf, how);
        if (!(membufflg(buf) & MEMBUF_EXT_BIT)) {
            memrelbuf(buf);

            return NULL;
        }
    }
    len -= membuftrailspace(buf);
    if (buf) {
        for (last = buf ; (membufnext(last)) ; last = membufnext(last)) {
            ;
        }
    }
    top = buf;
    tail = buf;
    while (len > 0) {
        buf = memgetbuf(how);
        if (!buf) {
            memrelchain(top);

            return NULL;
        }
        tail->hdr.next = buf;
        if (len > MEMBUF_PKT_LEN) {
            memallocext(buf, how);
            if (!(membufflg(buf) & MEMBUF_EXT_BIT)) {
                memrelchain(top);

                return NULL;
            }
        }
        tail = buf;
        len -= membuftrailspace(buf);
    }
    if (last != NULL) {
        last->hdr.next = top;
    }

    return buf;
}

/*
 * copy packet header from src to dest
 * - src->hdr.flg must have MEMBUF_PKTHDR_BIT set, and dest must be empty
 */
static __inline__ void
memcpypkthdr(struct membuf *src, struct membuf *dest)
{
    struct mempkthdr *spkt;
    struct mempkthdr *dpkt;

    spkt = &membufpkthdr(src);
    dpkt = &membufpkthdr(dest);
    dest->hdr.data = membufpktdata(src);
    dest->hdr.flg = spkt->flg & MEMBUF_PKT_COPY_BITS;
    *dpkt = *spkt;
    spkt->aux = NULL;

    return;
}

/* allocate new buffer to prepend to chain, copy data along */
static __inline__ struct membuf *
_membufprepend(struct membuf *buf, m_ureg_t len, long how)
{
    struct membuf *head;
    long           type;

    type = membuftype(buf);
    head = memgetbuf(how);
    if (!head) {
        memrelchain(buf);

        return NULL;
    }
    head->hdr.type = type;
    if (membufflg(buf) & MEMBUF_PKTHDR_BIT) {
        memcpypkthdr(buf, head);
        buf->hdr.flg &= ~MEMBUF_PKTHDR_BIT;
    }
    head->hdr.next = buf;
    if (len < MEMBUF_PKT_LEN) {
        membufalignpkt(head, len);
    }
    head->hdr.len = len;

    return head;
}

/* prepare buf for prepending len bytes of data */
static __inline__ struct membuf *
membufprepend(struct membuf *buf, m_ureg_t len, long how)
{
    struct mempkthdr *pkt;
    m_ureg_t          lead = membufleadspace(buf);

    if (lead >= len) {
        buf->hdr.data -= len;
        buf->hdr.len += len;
    } else {
        buf = _membufprepend(buf, len, how);
    }
    if ((buf) && (membufflg(buf) & MEMBUF_PKTHDR_BIT)) {
        pkt = &membufpkthdr(buf);
        pkt->len += len;
    }

    return buf;
}

/*
 * - copy len bytes from membuf chain starting from ofs0 bytes from the beginning
 * - if len is MEMBUF_COPY_ALL, copy to end of buf
 * - how can be 0 or MEM_WAIT
 * - copy is read-only; blocks are not copied, just reference counts incremented
 */
static __inline__ struct membuf *
memcpychain(struct membuf *src, m_ureg_t ofs0, m_ureg_t nb, long how)
{
    m_ureg_t           ofs = ofs0;
    long               cpyhdr = 0;
    m_ureg_t           len;
    long               type;
    struct membuf     *top = NULL;
    struct membuf     *dest;
    struct mempkthdr  *pkt;
    struct memexthdr  *ext1;
    struct memexthdr  *ext2;

    cpyhdr = !ofs && (membufflg(src) & MEMBUF_PKTHDR_BIT);
    while (ofs > 0) {
        len = membuflen(src);
        if (len < nb) {

            break;
        }
        nb -= len;
        src = membufnext(src);
    }
    while (nb > 0) {
        if (!src) {

            break;
        }
        type = membuftype(src);
        dest = memgetbuf(how);
        if (!top) {
            top = dest;
        }
        len = membuflen(src);
        if (!dest) {
            memrelchain(top);

            return NULL;
        }
        dest->hdr.type = type;
        if (cpyhdr) {
            pkt = &membufpkthdr(src);
            memcpypkthdr(dest, src);
            if (len == MEMBUF_COPY_ALL) {
                pkt->len -= ofs0;
            } else {
                pkt->len = len;
            }
            cpyhdr = 0;
        }
        dest->hdr.len = min(len, len - ofs);
        if (membufflg(src) & MEMBUF_EXT_BIT) {
            ext1 = &membufexthdr(src);
            ext2 = &membufexthdr(dest);
            dest->hdr.data = src->hdr.data + ofs;
            *ext2 = *ext1;
            dest->hdr.flg |= MEMBUF_EXT_BIT;
            membufincref(ext1);
        } else {
            kbcopy(membufpktdata(src) + ofs,
                   membufpktdata(dest),
                   membuflen(src));
        }
        if (len != MEMBUF_COPY_ALL) {
            len -= membuflen(src);
        }
        ofs = 0;
        dest = membufnext(dest);
        src = membufnext(src);
    }

    return top;
}

/*
 * - copy whole packet including header (which must be present)
 * - optimisation of memcpychain(buf, 0, M_COPY_ALL, how)
 * - how can be 0 or MEM_WAIT
 * - copy is read-only; blocks are not copied, just reference counts incremented
 * - preserve alignment of first membuf, e.g. for protocol headers
 */
static __inline__ struct membuf *
memcpypkt(struct membuf *src, long how)
{
    struct membuf    *dest;
    struct membuf    *buf;
    struct membuf    *top;
    struct memexthdr *ext1;
    struct memexthdr *ext2;
    m_ureg_t          len;
    uint8_t          *pkt1;
    uint8_t          *pkt2;
    long              type;

    type = membuftype(src);
    dest = memgetbuf(how);
    if (!dest) {

        return NULL;
    }
    top = dest;
    memcpypkthdr(src, dest);
    dest->hdr.len = membuflen(src);
    if (membufflg(src) & MEMBUF_EXT_BIT) {
        ext1 = &membufexthdr(src);
        ext2 = &membufexthdr(dest);
        dest->hdr.data = membufextdata(dest);
        *ext2 = *ext1;
        dest->hdr.flg |= MEMBUF_EXT_BIT;
        membufincref(ext1);
    } else {
        pkt1 = membufpktdata(src);
        pkt2 = membufpktdata(dest);
        dest->hdr.data = pkt1 + (src->hdr.data - pkt2);
        kbcopy(pkt1,
               pkt2,
               membuflen(src));
    }
    src = membufnext(src);
    while (src) {
        buf = memgetbuf(how);
        if (!buf) {
            memrelchain(top);

            return NULL;
        }
        dest->hdr.next = buf;
        len = membufpktlen(src);
        dest = buf;
        buf->hdr.len = len;
        if (membufflg(src) & MEMBUF_EXT_BIT) {
            ext1 = &membufexthdr(src);
            ext2 = &membufexthdr(dest);
            dest->hdr.data = membufextdata(src);;
            dest->hdr.flg |= MEMBUF_EXT_BIT;
            *ext2 = *ext1;
            membufincref(ext1);
        } else {
            kbcopy(membufpktdata(src),
                   membufpktdata(dest),
                   membufpktlen(src));
        }
        buf = membufnext(buf);
    }

    return top;
}

/*
 * copy data from buffer-chain starting at ofs bytes from the beginning into
 * the indicated buffer
 */
static __inline__ void
memcpydata(struct membuf *src, m_ureg_t ofs, m_ureg_t len, uint8_t *dest)
{
    m_ureg_t csz;

    csz = membuflen(src);
    while (ofs > 0) {
        if (ofs < csz) {

            break;
        }
        ofs -= csz;
        src = membufnext(src);
        len = membuflen(src);
    }
    if (len > 0) {
        csz = min(len - ofs, len);
        while (len > 0) {
            kbcopy(membufdata(src), dest, csz);
            len -= csz;
            src = membufnext(src);
            dest += csz;
            csz = membuflen(src);
        }
    }

    return;
}

/*
 * - copy whole packet including header into a new buffer-chain
 * - blocks are copied along
 * - like memcpypkt(), but produces a writable copy
 */
static __inline__ struct membuf *
memduppkt(struct membuf *src, long how)
{
    struct membuf  *dest;
    struct membuf  *top = NULL;
    uint8_t        *sptr;
    uint8_t        *dptr;
    long            type;
    m_ureg_t        ofs;
    m_ureg_t        len;
    long            sz;
    long            csz;

    if (!src) {

        return NULL;
    }
    len = membufpktlen(src);
    while (len > 0) {
        type = membuftype(src);
        dest = memgetbuf(how);
        if (!top) {
            top = dest;
        }
        if (!dest) {
            memrelchain(top);

            return NULL;
        }
        dest->hdr.type = type;
        if (!top) {
            memcpypkthdr(src, dest);
            csz = MEMBUF_PKT_LEN;
        } else {
            csz = MEMBUF_DATA_LEN;
        }
        if (len > MEMBUF_PKT_LEN) {
            dptr = memallocext(dest, how);
            if (!(membufflg(dest) & MEMBUF_EXT_BIT)) {
                memputbuf(dest);
                memrelchain(top);

                return NULL;
            }
            len = MEMBUF_BLK_SIZE;
        }
        csz = membuflen(src);
        dest = membufnext(dest);
        ofs = 0;
        sptr = membufpktdata(src);
        dptr = membufpktdata(dest);
        csz = min(csz, MEMBUF_PKT_LEN);
        kbcopy(sptr, dptr, csz);
        while ((len) && (src)) {
            dest->hdr.len = csz;
            sptr = membufpktdata(src);
            dptr += csz;
            len -= csz;
            kbcopy(sptr, dptr, csz);
            if (len) {
                src = membufnext(src);
            }
            ofs += csz;
            csz = membuflen(src);
            csz = min(csz, MEMBUF_PKT_LEN);
        }
        dest->hdr.len = len;
    }

    return top;
}

static __inline__ void
memcatchain(struct membuf *src, struct membuf *dest)
{
    uint8_t  *sptr;
    uint8_t  *dptr;
    uint8_t  *lim;
    long      join;
    m_ureg_t  slen;
    m_ureg_t  dlen;

    dptr = membufdata(dest);
    while (membufnext(dest)) {
        dest = membufnext(dest);
    }
    join = membufflg(dest);
    dlen = membuflen(dest);
    dptr += dlen;
    while (src) {
        slen = membuflen(src);
        lim = &dptr[MEMBUF_DATA_LEN];
        sptr = membufdata(src);
        if ((join)
            || dptr + slen >= lim) {
            dest->hdr.next = src;

            return;
        }
        kbcopy(sptr, dptr, slen);
        dptr += slen;
        src = memrelbuf(src);
    }
    dest->hdr.len = dlen;

    return;
}

