#include <mjolnir/mjolnir.h>

#define CHR_DEF_NAMES  5
static const char              *chrdefnames[MJOLNIR_CHR_CATEGORIES + 1][CHR_DEF_NAMES]
= {
    { NULL, NULL, NULL, NULL, NULL },               /* INVALID */
    { "toor", "loop", "pixel", "mage", "zero" },    /* PROGRAMMER */
    { "baud", "tap", "fiber", "storm", "krak" },    /* CRACKER */
    { "bug", "kaos", "trik", "dawn", "mek" },       /* CYBORG */
    { "max", "slash", "fury", "rage", "koma" },     /* SOLDIER */
    { "flash", "shine", "shadow", "snap", "piq" },  /* THIEF */
    { "hak", "core", "mage", "thunder", "tune" }    /* ENGINEER */
};

const char                     *chrcatnames[CHR_MAX_CATEGORY + 1]
= {
    NULL,
    "programmer",
    "cracker",
    "cyborg",
    "soldier",
    "thief",
    "engineer"
};

static const char *
chrgetname(long cat)
{
    const char                 *name;
    long                        ndx;

    ndx = getrandlim(MJOLNIR_CATEGORY_DEF_NAMES);
    name = chrdefnames[cat][ndx];

    return name;
}

void
chrinit(struct objchr *chr, long cat, const char *name)
{
    if (name) {
        chr->name = strdup(name);
    } else {
        name = chrgetname(cat);
    }
    chr->type = MJOLNIR_CHR_PLAYER;
    chr->cat = cat;
    chrinitkrak(chr);
    chr->budo.belt = MJOLNIR_WHITE_BELT;

    return;
}

