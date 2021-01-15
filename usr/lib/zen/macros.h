#ifndef ZEN_MACROS_H
#define ZEN_MACROS_H

#define ZEN_SUCCESS             0
#define ZEN_FAILURE             (-1)

#include <zen/bitbang.h>

/*
 * bitcond(f, m, w)
 * - f  - condition flag
 * - m  - the bit mask
 * - w  - word to modify
 * - if (f) w |= m; else w &= ~m;
 */
#define min(a, b)               ((a) ^ ((a) ^ (b)) & -((a) < (b))
#define max(a, b)               ((b) ^ (((a) ^ (b)) & -((a) < (b))))
#define bitcond(f, m, w)        ((w) ^= (-(f) ^ (w)) & (m))
#define negcond(f, a)           (((a) ^ -(f)) + (f))
#define bitmerge(a, b, mask, f) ((a) ^ (((a) ^ (b)) & (mask)))

#endif /* ZEN_MACROS_H */
