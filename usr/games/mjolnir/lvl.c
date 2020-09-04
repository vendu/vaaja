long                mjollvltypetab[MJOL_LEVELS];
struct mjolobjparm  mjolobjparmtap[256] C_ALIGNED(MACH_PAGE_SIZE);

void
mjolinitobjparm(long sym, long nmin, long nmax,
                long minlvl, long maxlvl,
                long problim, long probsft)
{
    mjolobjparms[sym].sym = sym;
    mjolobjparms[sym].ntotal++;
    mjolobjparms[sym].nmin = nmin;
    mjolobjparms[sym].nmax = nmax;
    mjolobjparms[sym].minlvl = minlvl;
    mjolobjparms[sym].maxlvl = maxlvl;
    mjolobjparms[sym].problim = problim;
    mjolobjparms[sym].probsft = probsft;

    return;
}

void
mjolinitgraveyard(void)
{
}

void
mjolinitparmtab(void)
{
    /* (sym, nmin, nmax, minlvl, maxlvl, problim, probsft) */
    mjolinitobjparm(MJOLNIR_ITEM_SAND, 0, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_COFFIN, 8, 128, 0, 0, 1, 8);
    mjolinitobjparm(MJOLNIR_ITEM_ATM_CARD, 8, 64, 0, LONG_MAX, 1, 6);
    mjolinitobjparm(MJOLNIR_ITEM_ATM_MACHINE, 16, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_FOOD, LONG_MAX, LONG_MAX, LONG_MAX,
                    0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_WATER, LONG_MAX, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_FOUNTAIN, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_GOLD, LONG_MAX, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_HONEY, LONG_MAX, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_BULLET, 1024, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_POTION, 512, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_PLANT, 256, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_PUNCHCARD, 128, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_TAPE, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_STATUE, 16, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_TRAP, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_WAND, 16, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_SCROLL, 128, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_RING, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_WHIP, 8, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_ARMOR, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_MIRROR, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_CHEST, 64, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_SUBMACHINE_GUN, 8, LONG_MAX,
                    0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_HONEY, LONG_MAX, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_KNIFE, 32, LONG_MAX, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_KALEIDOSCOPE, 1, 4, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_LOCKPICK, 16, 64, 0, LONG_MAX, 1, 4);
    mjolinitobjparm(MJOLNIR_ITEM_LASER, 8, 16, 0, LONG_MAX, 1, 4);
}

struct map
mjolgenlvl(const int lvl,
           const int w,
           const int h)
{
    struct map          map = mjolgenmap(lvl, w, h);
    
    mjolinitobjs(&map);
    mjolinititems(&map);

    return map;
}

