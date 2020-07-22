#define VAS_GLOBAL_NAME (1L << 0)

#define VAS_NAME_HASH_SLOTS     1024

/*
 * Macros
 * ------
 * TABHASH_TAB_T                hash subtable structure
 * TABHASH_ITEM_T               hash subtable item [structure]
 * TABHASH_SLOTS                number of slots in hash table
 * TABHASH_TAB_ITEMS            number of items in subtable
 * TABHASH_HASH(item)           item hash function for keys
 * TABHASH_CMP(item, key)       item comparator function
 * TABHASH_FREE(item)           item release function
 *
 * Operations
 * ----------
 * tabhashadd(hash, item)       add item to hash
 * tabhashfind(hash, key, rm)   find and optionally remove item from hash
 * tabhashrm(hash, key)         find and remove item from hash table
 * tabhashdel(hash, key)        remove and free item from hash table
 */
#define TABHASH_TAB_T           struct vasnametab
#define TABHASH_ITEM_T          struct vasname
#define TABHASH_ITEM_WORDS      4
#define TABHASH_SLOTS           VAS_NAME_HASH_SLOTS
#define TABHASH_KEY(item)       (item->str)
#define TABHASH_HASH(item)      vashashname(item->str)
#define TABHASH_CMP(item, key)  (strcmp(item->str, key))
#undef  TABHASH_FREE
#include <zen/tabhash.h>

struct vasname {
    uintptr_t   data;
    char       *str;
    char       *fname;
    size_t      len;
};

TABHASH_TAB_T  *g_nametab[VAS_NAME_HASH_SLOTS];
TABHASH_TAB_T **l_nametab;

long
vasinitnametab(void)
{
    void       *ptr;

    ptr = calloc(VAS_NAME_HASH_SLOTS, sizeof(TABHASH_TAB_T *));
    if (!ptr) {

        return -1;
    }
    l_nametab = ptr;

    return 0;
}

long
vasaddobj(struct vasname *name, void *data, long glob)
{
    long                ret = -1;
    struct vasnametab **hash;

    if (glob) {
        ret = tabhashadd(g_nametab, (const uintptr_t)name, (const uintptr_t)data);
    } else {
        ret = tabhashadd(l_nametab, (const uintptr_t)name, (const uintptr_t)data);
    }

    return ret;
}

long
vashashname(const char *str)
{
    const char         *cptr = str;
    size_t              len;
    long                ndx;

    len = 0;
    do {
        val += *cptr++;
        len++;
    } while (*cptr);
    ndx = tmhash32(val) & (VAS_NAME_HASH_SLOTS - 1);

    return ndx;
}

void
vashfreename(struct vasname *name)
{
    free(item->str);
    free(item->fname);
    free(item);

    return;
}
