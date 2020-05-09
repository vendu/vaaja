#ifndef __ZEN_TABHASH_H__
#define __ZEN_TABHASH_H__

/*
 * cache-friendly hash table
 * -------------------------
 * - instead of chaining single hash-items into linked lists, we chain tables
 *   of a few dozen items with a few words of book-keeping information in order
 *   to leverage massive cache-prefetching and avoid pointer-chasing/cache-
 *   fetching on per-item basis
 */

#include <string.h>
#include <mach/param.h>
#include <mt/lk.h>
#include <zero/trix.h>
#include <zen/sys/unix.h>
#include <zen/hash.h>

extern TABHASH_TAB_T   *TABHASH_BUF;

#define tabhashfind(hash, key)  tabhashop(hash, key, TABHASH_FIND)
#define tabhashrm(hash, key)    tabhashop(hash, key, TABHASH_REMOVE)
#define tabhashref(hash, key)   tabhashop(hash, key, TABHASH_ADD_REF)
#if defined(TABHASH_FREE)
#define tabhashdel(hash, key)   TABHASH_FREE(tabhashrm(hash, key))
#else
#define tabhashdel(hash, key)   tabhashrm(hash, key)
#endif
#if !defined(TABHASH_COPY)
#define TABHASH_COPY(src, dest) memcpy(dest, src, sizeof(TABHASH_ITEM_T))
#endif
#if !defined(TABHASH_CLEAR)
#define TABHASH_CLEAR(ptr)      memset(ptr, 0, sizeof(TABHASH_ITEM_T))
#endif

#define TABHASH_TAB_SIZE        (roundup2(sizeof(struct tabhashtab), CLSIZE))
#define TABHASH_BUF_SIZE        (2 * PAGESIZE)
#define TABHASH_CACHE_TABS      (TABHASH_BUF_SIZE / TABHASH_TAB_SIZE)
struct tabhashtab {
    long                ncur;
    long                nmax;
    TABHASH_TAB_T      *prev;
    TABHASH_TAB_T      *next;
    TABHASH_ITEM_T      items[TABHASH_TAB_ITEMS];
};

static __inline__ void
tabhashputtab(TABHASH_TAB_T *tab)
{
    TABHASH_TAB_T      *head;
    mtlkbit((m_atomic_t *)&TABHASH_BUF, MEM_LK_BIT_OFS);
    head = TABHASH_BUF;
    head = (void *)((uintptr_t)head & ~MEM_LK_BIT);
    if (head) {
        head->prev = tab;
    }
    tab->next = head;
    m_atomwrite((m_atomic_t *)&TABHASH_BUF, tab);

    return;
}

/* get hash subtable */
static __inline__ TABHASH_TAB_T *
tabhashgettab(void)
{
    TABHASH_TAB_T      *head = NULL;
    TABHASH_TAB_T      *tab;
    long                ntab;

    /* lock buffer */
    mtlkbit((m_atomic_t *)&TABHASH_BUF, MEM_LK_BIT_OFS);
    tab = TABHASH_BUF;
    tab = (void *)((uintptr_t)tab & ~MEM_LK_BIT);
    if (!tab) {
        int8_t         *ptr = mapanon(-1, TABHASH_BUF_SIZE, 0);
        TABHASH_TAB_T  *cur;
        TABHASH_TAB_T  *prev;

        /* allocate more subtables */
        if (ptr == MAP_FAILED) {
            tab = NULL;
            mtunlkbit((m_atomic_t *)&TABHASH_BUF, MEM_LK_BIT_OFS);
        } else {
            tab = (TABHASH_TAB_T *)ptr;         // first table
            ntab = TABHASH_CACHE_TABS - 1;
            ptr += TABHASH_TAB_SIZE;
            head = (TABHASH_TAB_T *)ptr;       // second table
            prev = head;
            while (--ntab) {
                /* chain newly-allocated table structures */
                ptr += TABHASH_TAB_SIZE;
                cur = (TABHASH_TAB_T *)ptr;
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
        memset(&tab->items, 0, sizeof(tab->items));
    }

    return tab;
}

static __inline__ long
tabhashadd(TABHASH_TAB_T **hashtab, const uintptr_t key, const uintptr_t val)
{
    TABHASH_TAB_T      *head;
    TABHASH_TAB_T      *tab;
    long                ndx;
    long                lim;
    long                loop;
    TABHASH_ITEM_T      item = { key, val };

    ndx = TABHASH_HASH(key);
    mtlkbit((m_atomic_t *)&hashtab[ndx], MEM_LK_BIT_OFS);
    head = hashtab[ndx];
    tab = (void *)((uintptr_t)head & ~MEM_LK_BIT);
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
            TABHASH_PUT_NREF(&tab->items[ndx], 1);
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
static __inline__ TABHASH_ITEM_T
tabhashop(TABHASH_TAB_T **hashtab, const uintptr_t val, long cmd)

{
    TABHASH_ITEM_T      ret = TABHASH_INVALID;
    TABHASH_TAB_T      *tab;
    TABHASH_TAB_T      *head;
    TABHASH_TAB_T      *prev;
    TABHASH_TAB_T      *next;
    TABHASH_ITEM_T     *item;
    long                ndx;
    long                n;
    long                nref;
    long                lim;
    long                cur;

    ndx = TABHASH_HASH(val);
    prev = NULL;
    mtlkbit((m_atomic_t *)&hashtab[ndx], MEM_LK_BIT_OFS);
    head = hashtab[ndx];
    tab = (void *)((uintptr_t)head & ~MEM_LK_BIT);
    head = tab;
    while (tab) {
        lim = tab->nmax;
        item = &tab->items[0];
        for (cur = 0 ; cur < lim ; cur++) {
            if (!TABHASH_CMP(item, val)) {
                TABHASH_COPY(item, &ret);
                nref = TABHASH_GET_NREF(item);
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
                    TABHASH_PUT_NREF(item, nref);
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
        prev = tab;
        tab = tab->next;
    }
    m_atomwrite((m_atomic_t *)&hashtab[ndx], head);

    return ret;
}

static __inline__ long
tabhashaddref(TABHASH_TAB_T **hashtab, const uintptr_t key, const uintptr_t val)
{
    TABHASH_ITEM_T      item = tabhashop(hashtab, val, TABHASH_ADD_REF);
    long                retval = 0;

    if (!TABHASH_CHK(&item)) {
        retval = tabhashadd(hashtab, key, val);
    } else {
        retval = 1;
    }

    return retval;
}

#endif /* __ZEN_TABHASH_H__ */

