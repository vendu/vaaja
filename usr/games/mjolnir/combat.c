#include <mjolnir/combat.h>
#include <dice/d20.h>

struct d20diceset               hitdiesets[MJOLNIR_RANKS];
long                            elementranks[MJOLNIR_ELEMENTS][MJOLNIR_ELEMENTS];
struct d20diceset               elementhitdice;
struct d20diceset               elementweakdice;
struct d20diceset               elementstrongdice;

void
chrsetwhite(struct objchr *chr)
{
    chr->str = 4;
    chr->prob = 2;
    chr->hitdice = &hitdiesets[MJOLNIR_WHITE_RANK];

    return 0;
}

void
chrsetorange(struct objchr *chr)
{
    chr->str = 8;
    chr->prob = 3;
    chr->hitdice = &hitdiesets[MJOLNIR_ORANGE_RANK];

    return 0;
}

void
chrsetblue(struct objchr *chr)
{
    chr->str = 12;
    chr->prob = 4;
    chr->hitdice = &hitdiesets[MJOLNIR_BLUE_RANK];

    return 0;
}

void
chrsetyellow(struct objchr *chr)
{
    chr->str = 16;
    chr->prob = 5;
    chr->hitdice = &hitdiesets[MJOLNIR_YELLOW_RANK];

    return 0;
}

void
chrsetgreen(struct objchr *chr)
{
    chr->str = 20;
    chr->prob = 6;
    chr->hitdice = &hitdiesets[MJOLNIR_GREEN_RANK];

    return 0;
}

void
chrsetbrown(struct objchr *chr)
{
    chr->str = 24;
    chr->prob = 7;
    chr->hitdice = &hitdiesets[MJOLNIR_BROWN_RANK];

    return 0;
}

void
chrsetblack(struct objchr *chr)
{
    chr->str = 32;
    chr->prob = 8;
    chr->hitdice = &hitdiesets[MJOLNIR_BLACK_RANK];

    return 0;
}

void
combatinitrank(long rank)
{
    struct d20diceset   *set = &hitdiesets[rank];

    switch (rank) {
        case MJOLNIR_WHITE_RANK:    // STR: 4, HIT: 2D6, PROB: 20
            set->d6 = 2;

            break;
        case MJOLNIR_ORANGE_RANK:   // STR: 8, HIT: 4D6, PROB: 30
            set->d6 = 4;

            break;
        case MJOLNIR_BLUE_RANK:     // STR: 12, HIT: 6D6, PROB: 40
            set->d6 = 6;

            break;
        case MJOLNIR_YELLOW_RANK:   // STR: 16, HIT: 8D6, PROB: 50
            set->d6 = 8;

            break;
        case MJOLNIR_GREEN_RANK:    // STR: 20, HIT: 10D6, PROB: 60
            set->d6 = 10;

            break;
        case MJOLNIR_BROWN_RANK:    // STR: 24, HIT: 12D6, PROB: 70
            set->d6 = 12;

            break;
        case MJOLNIR_BLACK_RANK:    // STR: 32, HIT: 16D6, PROB: 80
            set->d6 = 16;

            break;
    }

    return;
}

void
combatsetrank(struct objchr *chr, long lvl)
{
    long                        rank = lvl >> 1;

    if (rank != chr->rank && rank < MJOLNIR_RANKS) {
        switch (rank) {
            case MJOLNIR_WHITE_RANK:
                chrsetwhite(chr);

                break;
            case MJOLNIR_ORANGE_RANK:
                chrsetorange(chr);

                break;
            case MJOLNIR_BLUE_RANK:
                chrsetblue(chr);

                break;
            case MJOLNIR_YELLOW_RANK:
                chrsetyellow(chr);

                break;
            case MJOLNIR_GREEN_RANK:
                chrsetgreen(chr);

                break;
            case MJOLNIR_BROWN_RANK:
                chrsetbrown(chr);

                break;
            case MJOLNIR_BLACK_RANK:
                chrsetblack(chr);

                break;
        }
    }

    return;
}

/* FIXME */
void
combatinitelement(long elem)
{
    switch (elem) {
        case MJOLNIR_LAND_ELEMENT:
        case MJOLNIR_WATER_ELEMENT:
        case MJOLNIR_FIRE_ELEMENT:
        case MJOLNIR_AIR_ELEMENT:
    }

    return;
}

/* initialize table */
void
combatinit(void)
{
    combatinitrank(MJOLNIR_WHITE_RANK);
    combatinitrank(MJOLNIR_ORANGE_RANK);
    combatinitrank(MJOLNIR_BLUE_RANK);
    combatinitrank(MJOLNIR_YELLOW_RANK);
    combatinitrank(MJOLNIR_GREEN_RANK);
    combatinitrank(MJOLNIR_BROWN_RANK);
    combatinitrank(MJOLNIR_BLACK_RANK);

    return;
}

