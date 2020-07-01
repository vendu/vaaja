#include <stdlib.h>
#include "map.h"

int main(int argc, char *argv[])
{
    //    const struct map map = mapgen(80, 128, 8, 64);
    const struct map map = mapgen(80,
                                  80,
                                  12 + (rand() & 0x07),
                                  32 + (rand() & 0x1f));

    mapprint(map);
    mapclose(map);

    exit(0);
}
