#include <mjolnir/mjolnir.h>

#define CHR_INVALID_CATEGORY 0
#define CHR_PROGRAMMER_CAT   1
#define CHR_CRACKER_CAT      2
#define CHR_CYBORG_CAT       3
#define CHR_SOLDIER_CAT      4
#define CHR_THIEF_CAT        5
#define CHR_ENGINEER_CAT     6

struct chrcatname            chrcatnames[CHR_MAX_CATEGORY + 1]
= {
    { NULL, "NULL" },
    { "programmer", "loop" },
    { "cracker", "stak" },
    { "cyborg", "hak" },
    { "soldier", "flaz" },
    { "thief", "snax" },
    { "engineer", "tune" }
};

void
chrinit(struct objchr *chr, long cat, const char *name)
{
    if (!name) {
        chr->name = strdup(name);
    } else {
        name = chrcatnames[cat].def;
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

