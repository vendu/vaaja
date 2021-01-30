#ifndef GAMES_DICE_DIVMAGIC_H
#define GAMES_DICE_DIVMAGIC_H

#define modu5(val)              (val - val * divu5(val))

static C_INLINE unsigned long
divu5(unsigned long uval)
{
    unsigned long long mul = UINT64_C(0xcd);
    unsigned long long res = uval;
    unsigned long      cnt = 2;

    res *= mul;
    res >>= cnt;
    uval = (unsigned long)res;

    return uval;
}
#define modu20(val)             (val - val * divu20(val))

static C_INLINE unsigned long
divu20(unsigned long uval)
{
    unsigned long long mul = UINT64_C(0xcd);
    unsigned long long res = uval;
    unsigned long      cnt = 4;

    res *= mul;
    res >>= cnt;
    uval = (unsigned long)res;

    return uval;
}

#endif /* GAMES_DICE_DIVMAGIC_H */

