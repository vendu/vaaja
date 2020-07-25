/*
 * cache-friendly hash table
 * -------------------------
 * - instead of chaining single hash-items into linked lists, we chain tables
 *   of a few dozen items with a few words of book-keeping information in order
 *   to leverage massive cache-prefetching and avoid pointer-chasing/cache-
 *   fetching on per-item basis
 */

#include <string.h>
#include <zero/trix.h>
#include <zero/unix.h>
#include <zen/hash.h>
#include <mach/atomic.h>
#include <mach/param.h>
#include <mt/lk.h>

extern struct tabhashtab       *TABHASH_BUF;

#define tabhashfind(hash, key)  tabhashop(hash, key, TABHASH_FIND)
#define tabhashrm(hash, key)    tabhashop(hash, key, TABHASH_REMOVE)
#define tabhashref(hash, key)   tabhashop(hash, key, TABHASH_ADD_REF)
#if !defined(TABHASH_HASH)
#if (MACH_WORD_SIZE == 4)
#define TABHASH_HASH(u)         tmhash32(u)
#elif (MACH_WORD_SIZE == 8)
#define TABHASH_HASH(u)         tmhash64(u)
#endif
#endif
#if defined(TABHASH_FREE)
#define tabhashdel(hash, key)   TABHASH_FREE(tabhashrm(hash, key))
#else
#define tabhashdel(hash, key)   tabhashrm(hash, key)
#endif
#if !defined(TABHASH_COPY)
#define TABHASH_COPY(src, dest) memcpy((void *)dest,                    \
                                       (void *)src,                     \
                                       sizeof(struct tabhashitem))
#endif
#if !defined(TABHASH_BZERO)
#define TABHASH_BZERO(p, sz)    memset((void *)p, 0, sz)
#endif
#if !defined(TABHASH_CLEAR)
#define TABHASH_CLEAR(ptr)      TABHASH_BZERO(ptr, sizeof(struct tabhashitem))
#endif
#define TABHASH_ADD_NREF(p, n)  m_fetchadd((volatile m_atomic_t *)p, n)
#define TABHASH_READ_NREF(tab)  (tab->ncur)

#define TABHASH_TAB_SIZE        (roundup2(sizeof(struct tabhashtab),    \
                                          MACH_CL_SIZE))
#define TABHASH_BUF_SIZE        (4 * MACH_PAGE_SIZE)
#define TABHASH_CACHE_TABS      (TABHASH_BUF_SIZE / TABHASH_TAB_SIZE)

struct tabhashitem {
    intptr_t    key;
    intptr_t    val;
};

struct tabhashtab {
    volatile m_atomic_t             ncur;
    long                            nmax;
    volatile struct tabhashtab     *prev;
    volatile struct tabhashtab     *next;
    volatile struct tabhashitem     items[TABHASH_TAB_ITEMS];
};

static __inline__ void
tabhashputtab(volatile struct tabhashtab *tab)
{
    volatile struct tabhashtab *head;

    mtlkbit((m_atomic_t *)&TABHASH_BUF, MT_MEM_LK_BIT_OFS);
    head = TABHASH_BUF;
    head = (void *)((uintptr_t)head & ~MT_MEM_LK_BIT);
    if (head) {
        head->prev = tab;
    }
    tab->next = head;
    m_atomwrite((m_atomic_t *)&TABHASH_BUF, tab);

    return;
}

/* get hash subtable */
static __inline__ struct tabhashtab *
tabhashgettab(void)
{
    volatile struct tabhashtab *head = NULL;
    struct tabhashtab          *tab;
    long                       ntab;

    /* lock buffer */
    mtlkbit((m_atomic_t *)&TABHASH_BUF, MT_MEM_LK_BIT_OFS);
    tab = TABHASH_BUF;
    tab = (void *)((uintptr_t)tab & ~MT_MEM_LK_BIT);
    if (!tab) {
        int8_t                     *ptr = mapanon(-1, TABHASH_BUF_SIZE, 0);
        volatile struct tabhashtab *cur;
        volatile struct tabhashtab *prev;

        /* allocate more subtables */
        if (ptr == MAP_FAILED) {
            tab = NULL;
            mtunlkbit((m_atomic_t *)&TABHASH_BUF, MT_MEM_LK_BIT_OFS);
        } else {
            tab = (struct tabhashtab *)ptr;         // first table
            ntab = TABHASH_CACHE_TABS - 1;
            ptr += TABHASH_TAB_SIZE;
            head = (struct tabhashtab *)ptr;       // second table
            prev = head;
            while (--ntab) {
                /* chain newly-allocated table structures */
                ptr += TABHASH_TAB_SIZE;
                cur = (struct tabhashtab *)ptr;
                prev->next = cur;
                cur->prev = prev;
                prev = cur;
            }
            /* queue newly-allocated chain of tables */
            prev->next = NULL;
            m_atomwrite(&TABHASH_BUF, head);
        }
    } else {
        /* remove head from queue */
        head = tab->next;
        if (head) {
            head->prev = NULL;
        }
        m_atomwrite(&TABHASH_BUF, head);
    }
    if (tab) {
        tab->nmax = TABHASH_TAB_ITEMS;
        TABHASH_BZERO(&tab->items, sizeof(tab->items));
    }

    return tab;
}

static __inline__ long
tabhashadd(volatile struct tabhashtab **hashtab,
           const intptr_t key,
           const intptr_t val)
{
    volatile struct tabhashtab *tab;
    volatile struct tabhashtab *head;
    long                        ndx;
    long                        lim;
    long                        loop;
    struct tabhashitem          item = { key, (void *)val };

    ndx = TABHASH_HASH(key);
    mtlkbit((m_atomic_t *)&hashtab[ndx], MT_MEM_LK_BIT_OFS);
    head = hashtab[ndx];
    tab = (void *)((uintptr_t)head & ~MT_MEM_LK_BIT);
    head = tab;
    do {
        if (!tab) {
            /* allocate new subtable */
            tab = tabhashgettab();
            if (!tab) {

                return 0;
            }
            /* queue new subtable in front of chain */
            if (head) {
                head->prev = tab;
            }
            head = tab;
            tab->nmax = TABHASH_TAB_ITEMS;
        }
        ndx = tab->ncur;
        lim = tab->nmax;
        if (ndx < lim) {
            TABHASH_COPY(&item, &tab->items[ndx]);
            TABHASH_ADD_NREF(tab, 1);
            ndx++;
            tab->ncur = ndx;
            loop = 0;
        } else {
            tab = tab->next;
        }
    } while (loop);
    m_atomwrite((m_atomic_t *)&hashtab[ndx], head);

    return 1;
}

#define TABHASH_REMOVE          (-1L)
#define TABHASH_FIND            0
#define TABHASH_ADD_REF         (1L)
static __inline__ struct tabhashitem
tabhashop(volatile struct tabhashtab **hashtab, const intptr_t val, long cmd)

{
    struct tabhashitem              ret = TABHASH_INVALID;
    volatile struct tabhashtab     *tab;
    volatile struct tabhashtab     *head;
    volatile struct tabhashtab     *prev;
    volatile struct tabhashtab     *next;
    volatile struct tabhashitem    *item;
    intptr_t                        key;
    intptr_t                        val;
    intptr_t                        mask;
    long                            ndx;
    long                            n;
    long                            nref;
    long                            lim;
    long                            cur;

    ndx = TABHASH_HASH(val);
    prev = NULL;
    mtlkbit((m_atomic_t *)&hashtab[ndx], MT_MEM_LK_BIT_OFS);
    head = hashtab[ndx];
    tab = (void *)((uintptr_t)head & ~MT_MEM_LK_BIT);
    head = tab;
    while (tab) {
        lim = tab->nmax;
        item = &tab->items[0];
        if (cmd == TABHASH_REMOVE) {
            n = tab->ncur;
            lim = max(8, n);
            switch (lim) {
                case 8:
                    item = &tab->items[7];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 7:
                    item = &tab->items[6];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 6:
                    item = &tab->items[5];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 5:
                    item = &tab->items[4];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 4:
                    item = &tab->items[3];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 3:
                    item = &tab->items[2];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 2:
                    item = &tab->items[1];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 1:
                    item = &tab->items[0];
                    mask = -(item->key == key);
                    mask &= (intptr_t)item;
                    val |= mask;
                case 0:

                    break;
            }
            if (val) {
                ret = *(struct tabhashitem *)val;

                break;
            }
        } else {
            for (cur = 0 ; cur < lim ; cur++) {
                if (TABHASH_CMP(item, val)) {
                    TABHASH_COPY(item, &ret);
                    nref = TABHASH_READ_NREF(tab);
                    if (cmd == TABHASH_REMOVE && nref == 1) {
                        /* remove item from hash */
                        n = tab->ncur;
                        n--;
                        if (cur != n) {
                            /* copy last item in table over removed one */
                            TABHASH_COPY(&tab->items[n], &tab->items[cur]);
                        }
#if defined(TABHASH_FREE)
                        TABHASH_FREE(tab->items[n]);
#endif
                        TABHASH_CLEAR(&tab->items[n]);
                        tab->ncur = n;
                        prev = tab->prev;
                        next = tab->next;
                        if (!n) {
                            /* free empty table */
                            if (prev) {
                                prev->next = next;
                            } else {
                                head = next;
                            }
                            if (next) {
                                next->prev = prev;
                            }
                            tabhashputtab(tab);
                        }
                    } else {
                        nref += cmd;
                        TABHASH_ADD_NREF(tab, nref);
                        if (prev) {
                            /* insert table into front of queue */
                            next = tab->next;
                            prev->next = next;
                            if (next) {
                                next->prev = prev;
                            }
                            tab->next = hashtab[ndx];
                        }
                        head = tab;
                    }
                    m_atomwrite((m_atomic_t *)&hashtab[ndx], head);
                    
                    return ret;
                }
                item++;
            }
        }
        prev = tab;
        tab = tab->next;
    }
    m_atomwrite((m_atomic_t *)&hashtab[ndx], head);

    return ret;
}

static __inline__ long
tabhashaddref(volatile struct tabhashtab **hashtab,
              const intptr_t key,
              const intptr_t val)
{
    struct tabhashitem      item = tabhashop(hashtab, val, TABHASH_ADD_REF);
    long                retval = 0;

    if (!TABHASH_CHK(&item)) {
        retval = tabhashadd(hashtab, key, val);
    } else {
        retval = 1;
    }

    return retval;
}

