#ifndef __MJOLNIR_UTIL_H__
#define __MJOLNIR_UTIL_H__

#define seedrand(seed)  srandmt32(seed)
#define getrand()       (randmt32() & 0x7fffffff)
#define getrandlim(lim) (getrand() % lim)

#endif /* __MJOLNIR_UTIL_H__ */

