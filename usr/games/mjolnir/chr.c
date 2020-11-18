#include <stdio.h>
#include <string.h>
#include <mjolnir/obj.h>

const char             *chrcatnames[CHR_MAX_CATEGORY + 1]
= {
    NULL,
    "programmer",
    "cracker",
    "cyborg",
    "thief",
    "engineer"
};

/* per-category ice skill initialization */
void
chrinitice(struct objchr *chr)
{
    struct ice                 *ice = &chr->ice;

    switch(chr->cat) {
        case CHR_PROGRAMMER_CATEGORY:
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 128;
            ice->maxhp = 128;
            ice->nhp = 128;
            ice->defdie = 20;
            ice->hitdie = 12;

            break;
        case CHR_CRACKER_CATEGORY:
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 96;
            ice->maxhp = 96;
            ice->nhp = 96;
            ice->defdie = 12;
            ice->hitdie = 10;

            break;
        case CHR_CYBORG_CATEGORY:
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 32;
            ice->maxhp = 32;
            ice->nhp = 32;
            ice->defdie = 8;
            ice->hitdie = 6;

            break;
        case CHR_THIEF_CATEGORY:
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 32;
            ice->maxhp = 32;
            ice->nhp = 32;
            ice->defdie = 6;
            ice->hitdie = 4;

            break;
        case CHR_ENGINEER_CATEGORY:
            ice->xp = 0;
            ice->lvl = 1;
            ice->basehp = 64;
            ice->maxhp = 64;
            ice->nhp = 64;
            ice->defdie = 10;
            ice->hitdie = 8;

            break;
        default:
            fprintf(stderr, "invalid character category %ld\n", chr->cat);

            break;
    }
}

void
chrinit(struct objchr *chr, const char *name, long type, long cat)
{
    chr->name = strdup(name);
    chr->type = type;
    chr->cat = cat;
    chrinitice(chr);

    return;
}

