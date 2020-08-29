#include <mjolnir/conf.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

void
dungenlvl(int w, int h, int gridsize, int maxrooms)
{
    //    const struct map map = mapgen(80, 128, 8, 64);
    const struct map map = dungenmap(1,
                                     w,
                                     h,
                                     gridsize,
                                     maxrooms);
    dungenprint(map);
    dungenclose(map);

    return;
}

#if defined(TEST_DUNGEN)
int
main(int argc, char *argv[])
{
    mjolsrand(time(NULL));
    dungenlvl(80,
              //              48 + (mjolrand() & 0x1f),
              48,
              8 + (mjolrand() & 0x07),
              //              4 + (rand() & 0x07),
              16 + (mjolrand() & 0x1f));

    exit(0);
}
#endif

