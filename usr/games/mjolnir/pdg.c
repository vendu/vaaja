#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mjolnir/pdg.h>

/*
 * REFERENCE: https://gamedevelopment.tutsplus.com/tutorials/generate-random-cave-levels-using-cellular-automata--gamedev-9664
 */

struct map *
pdginitmap(struct map *map, int w, int h)
{
    mapitem_t  *tab;
    int         seed = time(NULL);

    pdgseed(seed);
    if (!map) {
        map = malloc(sizeof(struct map));
        if (!map) {
            fprintf(stderr, "PDG: cannot allocate map-structure\n");
            
            abort();
        }
    }
    tab = calloc(w * h, sizeof(mapitem_t));
    if (!tab) {
        fprintf(stderr, "PDG: cannot allocate map-table\n");

        abort();
    }
    map->seed = seed;
    map->lifeprob = PDG_DEF_LIFE_PROB;
    map->deathlim = PDG_DEF_DEATH_LIMIT;
    map->birthmin = PDG_DEF_BIRTH_MIN;
    map->w = w;
    map->h = h;
    map->tab = tab;

    return map;
}

#if 0

#define pdgmkcell() (pdgprob(0x0000ffff) ? ' ' : '#')

void
pdgaddrand(struct map *map)
{
    int n = (map->w * map->h) >> 2;
    int x;
    int y;

    while (n--) {
        x = mjolrand() % map->w;
        y = mjolrand() & map->h;
        map->tab[y * map->h + x] = pdgmkcell();
    }

    return;
}

void
pdgaddcells(struct map *map)
{
    int dx = map->w >> 5;
    int dy = map->h >> 4;
    int n = 15;
    int x = dx;
    int y = dy;

    while (n) {
        n--;
        map->tab[y * map->h + x] = pdgmkcell();
        map->tab[y * map->h + x + 1] = pdgmkcell();
        map->tab[y * map->h + x - 1] = pdgmkcell();
        map->tab[(y - 1) * map->h + x] = pdgmkcell();
        map->tab[(y - 1) * map->h + x + 1] = pdgmkcell();
        map->tab[(y - 1) * map->h + x - 1] = pdgmkcell();
        map->tab[(y + 1) * map->h + x] = pdgmkcell();
        map->tab[(y + 1) * map->h + x + 1] = pdgmkcell();
        map->tab[(y + 1) * map->h + x - 1] = pdgmkcell();
        x += dx;
        map->tab[y * map->h + x] = pdgmkcell();
        map->tab[y * map->h + x + 1] = pdgmkcell();
        map->tab[y * map->h + x - 1] = pdgmkcell();
        map->tab[(y - 1) * map->h + x] = pdgmkcell();
        map->tab[(y - 1) * map->h + x + 1] = pdgmkcell();
        map->tab[(y - 1) * map->h + x - 1] = pdgmkcell();
        map->tab[(y + 1) * map->h + x] = pdgmkcell();
        map->tab[(y + 1) * map->h + x + 1] = pdgmkcell();
        map->tab[(y + 1) * map->h + x - 1] = pdgmkcell();
        x += dx;
        y += dy;
    }
    pdgprintmap(map);

    return;
}

#endif /* 0 */

void
pdgfillmap(struct map *map)
{
    mapitem_t  *tab = map->tab;
    int         prob = map->lifeprob;
    int         x;
    int         y;

    for (y = 0 ; y < map->h ; y++) {
        for (x = 0 ; x < map->w ; x++) {
            if (pdgprob(prob)) {
                *tab = ' ';
            } else {
                *tab = '#';
            }
            tab++;
        }
    }

    return;
}

int
pdgcntlivenbs(struct map *map, int x, int y)
{
    int         cnt = 0;
    int         i;
    int         j;

    for (i = -1 ; i <= 1 ; i++) {
        for (j = -1 ; j <= 1 ; j++) {
            int nx = x + i;
            int ny = y + j;

            /* skip ourselves */
            if ((i) || (j)) {
                if (nx < 0 || ny < 0 || nx >= map->w || ny > map->h) {
                    /* index is off the map */
                    cnt++;
                } else if (map->tab[ny * map->w + nx] == ' ') {
                    cnt++;
                }
            }
        }
    }

    return cnt;
}

void
pdggrowmap(struct map *map)
{
    mapitem_t  *origtab = map->tab;
    mapitem_t  *oldtab = origtab;
    mapitem_t  *tab = calloc(map->w * map->h, sizeof(mapitem_t));
    int         prob;
    int         lim;
    int         x;
    int         y;

    if (!tab) {
        fprintf(stderr, "failed to allocate new map-table\n");

        abort();
    }
    map->tab = tab;
    for (y = 0 ; y < map->h ; y++) {
        for (x = 0 ; x < map->w ; x++) {
            int nbs = pdgcntlivenbs(map, x, y);

            if (*oldtab == ' ') {
                if (nbs < map->deathlim) {
                    *tab = '#';
                } else {
                    *tab = ' ';
                }
                *tab = ' ';
            } else if (nbs == map->birthmin) {
                *tab = ' ';
            } else {
                *tab = '#';
            }
            oldtab++;
            tab++;
        }
    }
    free(origtab);

    return;
}

void
pdgfilledges(struct map *map)
{
    mapitem_t  *tab = map->tab;
    int         x;
    int         y;

    /* top edge */
    for (x = 0 ; x < map->w ; x++) {
        *tab = '#';
        tab++;
    }
    /* bottom edge */
    y = map->h - 1;
    tab = &map->tab[y * map->w];
    for (x = 0 ; x < map->w ; x++) {
        *tab = '#';
        tab++;
    }
    /* left edge */
    x = 0;
    tab = &map->tab[map->w];
    for (y = 1 ; y < map->h ; y++) {
        *tab = '#';
        tab += map->w;
    }
    /* right edge */
    x = map->w - 1;
    tab = &map->tab[x];
    for (y = 1 ; y < map->h ; y++) {
        *tab = '#';
        tab += map->w;
    }

    return;
}

struct map *
pdggenmap(struct map *map, int w, int h, int nstep)
{
    map = pdginitmap(map, w, h);
    pdgfillmap(map);
    while (nstep--) {
        pdggrowmap(map);
    }
    pdgfilledges(map);

    return map;
}

void
pdgprintmap(struct map *map)
{
    mapitem_t  *tab = map->tab;
    int         x;
    int         y;

    for (y = 0 ; y < map->h ; y++) {
        for (x = 0 ; x < map->w ; x++) {
            printf("%c", *tab);
            tab++;
        }
        printf("\n");
    }

    return;
}

#if defined(TEST_PDG)
int
main(int argc, char *argv[])
{
    struct map *map = pdggenmap(NULL, 80, 24, 1);

    pdgprintmap(map);
    
    exit(0);
}
#endif /* TEST_PDG */

