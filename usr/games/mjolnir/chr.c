#include <mjolnir/mjolnir.h>

struct chrcatname               chrcatnames[CHR_MAX_CATEGORY + 1]
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
    iceinitchr(chr, name, cat);

    return;
}

