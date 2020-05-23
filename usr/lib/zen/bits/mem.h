#ifndef __ZEN_BITS_MEM_H__
#define __ZEN_BITS_MEM_H__

#include <mach/param.h>
#include <zen/hash.h>
#include <zen/tabhash.h>
#include <zen/sys/unix.h>

static __inline__ void *
memgethashtab(void)
{
    volatile struct tabhashtab  **tabq;
    long                          ntry = 32;
    size_t                        tabsz;
    size_t                        ntab;
    struct tabhashtab            *ret;
    int8_t                       *ptr;
    struct tabhashtab            *tab;
    struct tabhashtab            *head;
    struct tabhashtab            *prev;

    ret = NULL;
    ptr = MAP_FAILED;
    tabsz = TABHASH_TAB_SIZE;
    ntab = TABHASH_CACHE_TABS;
    tabq = &g_mem.hashtabq;
    do {
        mtlkbit((m_atomic_t *)tabq, MEM_LK_BIT_OFS);
        ret = (void *)((uintptr_t)*tabq & ~MEM_LK_BIT);
        if (!ret) {
            if (ptr == MAP_FAILED) {
                /* map header region */
                ptr = mapanon(-1, ntab * tabsz, 0);
                if (ptr != MAP_FAILED) {
                    tab = (struct tabhashtab *)ptr;
                    ret = (struct tabhashtab *)ptr;
                    head = (struct tabhashtab *)((int8_t *)tab + tabsz);
                    head->prev = NULL;
                    prev = head;
                    while (--ntab) {
                        tab = (struct tabhashtab *)((int8_t *)tab + tabsz);
                        tab->prev = prev;
                        prev->next = tab;
                        prev = tab;
                    }
                    tab->next = NULL;
                }
            }
            if (ptr != MAP_FAILED) {
                ret = (struct tabhashtab *)ptr;
            }
        }
        if (ret) {
            head = ret->next;
            if (head) {
                head->prev = NULL;
            }
        }
        m_atomwrite((m_atomic_t *)tabq, (m_atomic_t *)head);
        if (!ret) {
            m_waitspin();
        }
    } while (!ret && --ntry);

    return ret;
}

#endif /* __ZEN_BITS_MEM_H__ */

