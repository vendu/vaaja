#include <mjolnir/budo.h>
#include <dice/d20.h>

struct d20diceset               budohitdice[MJOLNIR_RANKS];
long                            elementranks[MJOLNIR_ELEMENTS][MJOLNIR_ELEMENTS];
struct d20diceset               elementhitdice;
struct d20diceset               elementweakdice;
struct d20diceset               elementstrongdice;

void
chrsetbudorank(struct objchr *chr, long rank)
{
    switch (rank) {
        case MJOLNIR_WHITE_RANK:
            chr->budo.str = 4;
            chr->budo.hitprob = 10;
            chr->budo.hitdice = budohitdice[MJOLNIR_WHITE_RANK];

            break;
        case MJOLNIR_ORANGE_RANK:
            chr->budo.str = 8;
            chr->budo.hitprob = 30;
            chr->budo.hitdice = budohitdice[MJOLNIR_ORANGE_RANK];

            break;
        case MJOLNIR_BLUE_RANK:
            chr->budo.str = 12;
            chr->budo.hitprob = 40;
            chr->budo.hitdice = budohitdice[MJOLNIR_BLUE_RANK];

            break;
        case MJOLNIR_YELLOW_RANK:
            chr->budo.str = 16;
            chr->budo.hitprob = 5;
            chr->budo.hitdice = budohitdice[MJOLNIR_YELLOW_RANK];

            break;
        case MJOLNIR_GREEN_RANK:
            chr->budo.str = 20;
            chr->budo.hitprob = 50;
            chr->budo.hitdice = budohitdice[MJOLNIR_GREEN_RANK];

            break;
        case MJOLNIR_BROWN_RANK:
            chr->budo.str = 24;
            chr->budo.hitprob = 60;
            chr->budo.hitdice = budohitdice[MJOLNIR_BROWN_RANK];

            break;
        case MJOLNIR_BLACK_RANK:
            chr->budo.str = 32;
            chr->budo.hitprob = 70;
            chr->budo.hitdice = budohitdice[MJOLNIR_BLACK_RANK];
    }

    return;
}

void
budoinitrank(long rank)
{
    struct d20diceset   *set = &budohitdice[rank];

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
        default:
            fprintf(stderr, "invalid rank %ld for budohitdice");

            exit(1);
    }

    return;
}

void
budosetrank(struct objchr *chr, long lvl)
{
    long                        rank = lvl >> 1;

    if (rank != chr->budo.rank && rank < MJOLNIR_RANKS) {
        chrsetbudorank(ch, rank);
    }

    return;
}

/*
 * ATTACK
 * ------
 * - LJND is strong vs air, neutral vs water and land, weak vs fire
 * - WATER is strong vs fire, neutral vs water and land, weak vs air
 * - FIRE is strong vs land, neutral vs fire and air,  weak vs water
 * - AIR is strong vs firer, neutral vs land and air, veak vs water
 */
void
combatinitelement(long elem)
{
    switch (elem) {
        /* strong: air, neutral: water, land, weak: fire */
        case MJOLNIR_LAND_ELEMENT:
        case MJOLNIR_WATER_ELEMENT:
        case MJOLNIR_FIRE_ELEMENT:
        case MJOLNIR_AIR_ELEMENT:
    }

    return;
}

/* initialize table */
void
budoinit(void)
{
    long                        rank;

    for (rank = MJOLNIR_WHITE_RANK ; rank <= MJOLNIR_BLACK_RANK ; rank++) {
        budoinitrank(rank);
    }

    return;
}

