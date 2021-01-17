#include <mjolnir/mjolnir.h>

#define CHR_INVALID_CATEGORY    0
#define CHR_PROGRAMMER_CATEGORY 1
#define CHR_CRACKER_CAT         2
#define CHR_CYBORG_CAT          3
#define CHR_SOLDIER_CAT         4
#define CHR_THIEF_CAT           5
#define CHR_ENGINEER_CAT        6

#define CHR_CATEGORY_DEF_NAMES  4
static const char              *chrdefnames[CHR_MAX_CATEGORY + 1][CHR_CATEGORY_DEF_NAMES]
= {
    { NULL, NULL, NULL, NULL },             /* INVALID */
    { "toor", "loop", "pixel", "mage" },    /* PROGRAMMER */
    { "baud", "tap", "fiber", "storm" },    /* CRACKER */
    { "bug", "kaos", "trik", "dawn" },      /* CYBORG */
    { "max", "slash", "fury", "rage" },     /* SOLDIER */
    { "flash", "shine", "shadow", "snap" }, /* THIEF */
    { "hak", "core", "mage", "thunder" }    /* ENGINEER */
};

static long                     chrcounts[CHR_MAX_CATEGORY + 1]
= {
    0,
    0,
    0,
    0,
    0,
    0,
    0
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
    long                        cnt;

    cnt = chrcounts[cat];
    if (cnt < CHR_CATEGORY_DEF_NAMES) {
        name = chrdefnames[cat][cnt];
        cnt++;
        chrcounts[cat] = cnt;
    } else {
        printf("%s: no default names left\n", chrcatnames[cat]);

        exit(1);
    }

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
    if (cat == CHR_CYBORG_CATEGORY) {
        chr->type = CHR_CYBORG;
    } else {
        chr->type = CHR_HUMAN;
    }
    chr->cat = cat;
    iceinitchr(chr);

    return;
}

