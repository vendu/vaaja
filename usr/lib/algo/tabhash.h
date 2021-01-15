/*
 * cache-friendly hash table
 * -------------------------
 * - instead of chaining single hash-items into linked lists, we chain tables
 *   of a few dozen items with a few words of book-keeping information in order
 *   to leverage massive cache-prefetching and avoid pointer-chasing/cache-
 *   fetching on per-item basis
 */

#include <string.h>
#include <env/trix.h>
#include <env/unix.h>
#include <zen/hash.h>
#include <mach/atomic.h>
#include <mach/param.h>
#include <mt/lk.h>

extern struct tabhashtab           *TABHASH_TAB[TABHASH_SLOTS];
extern volatile struct tabhashtab  *TABHASH_BUF;

#define TABHASH_INVALID             (struct tabhashitem){ 0, 0 }

#define tabhashfind(hash, key)      tabhashop(key, TABHASH_FIND)
#define tabhashdel(hash, key)       tabhashop(key, TABHASH_REMOVE)
#if !defined(TABHASH_HASH)
#if (MACH_WORD_SIZE == 4)
#define TABHASH_HASH(u)             tmhash32(u)
#elif (MACH_WORD_SIZE == 8)
#define TABHASH_HASH(u)             tmhash64(u)
#endif
#endif
#if !defined(TABHASH_COPY)
#define TABHASH_COPY(src, dest)     memcpy((void *)dest,                \
                                           (void *)src,                 \
                                           sizeof(struct tabhashitem))
#endif
#if !defined(TABHASH_CLEAR)
#define TABHASH_ZERO(ptr)           (*(ptr) = TABHASH_INVALID)
#endif
#define TABHASH_TAB_SIZE            sizeof(struct tabhashtab)
#define TABHASH_BUF_SIZE            (4 * MACH_PAGE_SIZE)
#define TABHASH_CACHE_TABS          (TABHASH_BUF_SIZE / TABHASH_TAB_SIZE)

struct tabhashitem {
    uintptr_t   key;
    uintptr_t   val;
};

struct tabhashtab {
    m_atomic_t                      ncur;
    m_word_t                        nmax;
    struct tabhashtab              *prev;
    struct tabhashtab              *next;
    struct tabhashitem              items[TABHASH_TAB_ITEMS];
};

static __inline__ void
tabhashputtab(struct tabhashtab *buf, struct tabhashtab *tab)
{
    struct tabhashtab  *head;

    mtlkbit((m_atomic_t *)&buf, MT_ADR_LK_BIT_OFS);
    head = (void *)((uintptr_t)&buf & ~MT_ADR_LK_BIT);
    tab->next = head;
    m_atomwrite((m_atomic_t *)&buf, (m_atomic_t *)tab);

    return;
}

/* get hash subtable */
static __inline__ struct tabhashtab *
tabhashgettab(struct tabhashtab *buf)
{
    struct tabhashtab  *tab;
    struct tabhashtab  *head;
    struct tabhashtab  *prev;
    struct tabhashtab  *cur;
    m_word_t            ntab;
    int8_t             *ptr;

    /* lock buffer */
    mtlkbit((m_atomic_t *)&buf, MT_ADR_LK_BIT_OFS);
    tab = (void *)((uintptr_t)buf & ~MT_ADR_LK_BIT);
    if (!tab) {
    /* allocate more subtables */
        ptr = mapanon(-1, buf_SIZE, 0);
        if (ptr == MAP_FAILED) {
            tab = NULL;
        } else {
            tab = (struct tabhashtab *)ptr;         // first table
            tab->nmax = TABHASH_TAB_ITEMS - 1;
            ptr += TABHASH_TAB_SIZE;
            head = (struct tabhashtab *)ptr;       // second table
            prev = head;
            while (--ntab) {
                /* chain newly-allocated table structures */
                ptr += TABHASH_TAB_SIZE;
                cur = (struct tabhashtab *)ptr;
                prev->next = cur;
                cur->nmax = TABHASH_TAB_ITEMS;
                prev = cur;
                /* queue newly-allocated chain of tables */
                prev->next = NULL;
            }
        }
    } else {
        /* remove head from stack/queue */
        head = tab->next;
    }
    m_atomwrite((m_atomic_t *)&buf, (m_atomic_t *)head);

    return tab;
}

static __inline__ m_word_t
tabhashadd(struct tabhashtab **hash,
           const uintptr_t key,
           const uintptr_t val)
{
    struct tabhashtab  *tab;
    struct tabhashtab  *head;
    m_word_t            ndx;
    struct tabhashitem  item = { key, val };

    ndx = TABHASH_HASH(key);
    mtlkbit((m_atomic_t *)&hash[ndx], MT_ADR_LK_BIT_OFS);
    head = hash[ndx];
    tab = (void *)((uintptr_t)head & ~MT_ADR_LK_BIT);
    head = tab;
    if (!tab || tab->ncur == tab->nmax) {
        /* allocate new subtable */
        tab = tabhashgettab();
        if (!tab) {

            return 0;
        }
        tab->ncur = 1;
        tab->nmax = TABHASH_TAB_ITEMS;
        tab->next = head;
        tab->items[0] = item;
    } else {
        ndx = tab->ncur;
        TABHASH_COPY(&item, &tab->items[ndx]);
        ndx++;
        tab->ncur = ndx;
    }
    m_atomwrite((m_atomic_t *)&hash[ndx], (m_atomic_t *)head);

    return 1;
}

#define TABHASH_REMOVE          (-1L)
#define TABHASH_FIND            0
static __inline__ struct tabhashitem
tabhashop(struct tabhash **hash,
          uintptr_t val,
          m_word_t cmd)

{
    struct tabhashitem  ret = TABHASH_INVALID;
    struct tabhashtab  *tab;
    struct tabhashtab  *head;
    struct tabhashtab  *prev;
    struct tabhashtab  *next;
    struct tabhashitem *item;
    uintptr_t           key;
    uintptr_t           mask;
    m_word_t            ndx;
    m_word_t            n;
    m_word_t            lim;

    ndx = TABHASH_HASH(val);
    prev = NULL;
    mtlkbit((m_atomic_t *)hash[ndx], MT_ADR_LK_BIT_OFS);
    head = hash[ndx];
    tab = (void *)((uintptr_t)head & ~MT_ADR_LK_BIT);
    head = tab;
    prev = NULL;
    while (tab) {
        lim = tab->nmax;
        item = &tab->items[0];
        if (cmd <= TABHASH_FIND) {
            n = tab->ncur;
            lim = max(8, n);
            switch (lim) {
                case 8:
                    item = &tab->items[7];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 7:
                    item = &tab->items[6];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 6:
                    item = &tab->items[5];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 5:
                    item = &tab->items[4];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 4:
                    item = &tab->items[3];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 3:
                    item = &tab->items[2];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 2:
                    item = &tab->items[1];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 1:
                    item = &tab->items[0];
                    mask = -(item->key == key);
                    mask &= (uintptr_t)item;
                    val |= mask;
                case 0:
                default:

                    break;
            }
            if (val) {
                item = (struct tabhashitem *)val;
                n = tab->ncur;
                if (cmd == TABHASH_REMOVE) {
                    if (n == 1) {
                        next = tab->next;
                        prev = tab->prev;
                        if (next) {
                            next->prev = prev;
                        }
                        if (prev) {
                            prev->next = next;
                        } else {
                            head = next;
                        }
                        TABHASH_ZERO(&tab->items[n]);
                        tab->ncur = 0;
                        tabhashputtab(tab);
                    } else {
                        TABHASH_COPY(&tab->items[n], item);
                        TABHASH_ZERO(&tab->items[n]);
                        n--;
                        tab->ncur = n;
                    }
                }

                break;
            }
            n -= lim;
            prev = tab;
            tab = tab->next;
        }
    }
    m_atomwrite((m_atomic_t *)&hash[ndx], (m_atomic_t *)head);

    return ret;
}

