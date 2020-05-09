#include <stdlib.h>
#include <limits.h>
#include <zero/maplk.h>

zeromaplk *
maplkinit(zeromaplk *maplk, unsigned long n)
{
    long       own = 0;
    zeromaplk *map;
    void      *bits;

    if (maplk) {
        map = maplk;
    } else {
        map = malloc(sizeof(zeromaplk));
        if (!map) {

            return NULL;
        }
        own++;
    }
    bits = calloc(n, sizeof(long) / CHAR_BIT);
    if (!bits && (own)) {
        free(maplk);

        return NULL;
    }
    maplk->nbit = n;
    maplk->bits = bits;

    return maplk;
}

