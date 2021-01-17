#ifndef DECK_BITMAGIC_H
#define DECK_BITMAGIC_H

#define min(a, b)               ((a) <= (b) ? (a) : (b))
#define max(a, b)               ((a) >= (b) ? (a) : (b))

#define min2(a, b)              ((b) ^ (((a) ^ (b)) & -((a) < (b))))
#define max2(a, b)              ((a) ^ (((a) ^ (b)) & -((a) < (b))))

#define haszerob(u)             (((u) - 0x01010101U) & ~(u) & 0x80808080U)

#endif /* DECK_BITMAGIC_H */

