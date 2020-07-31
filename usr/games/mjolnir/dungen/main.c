#include <stdlib.h>
#include "map.h"

void
dungen(int w, int h, int gridsize, int maxrooms)
{
    //    const struct map map = mapgen(80, 128, 8, 64);
    const struct map map = mapgen(w,
                                  h,
                                  gridsize,
                                  maxrooms);
    mapprint(map);
    mapclose(map);

    return;
}

#if defined(TEST_DUNGEN)
int
main(int argc, char *argv[])
{
    dungen(80,
           96 + (rand() & 0x1f),
           10 + (rand() & 0x07),
           64 + (rand() & 0x1f));

    exit(0);
}
#endif

