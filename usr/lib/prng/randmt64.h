#ifndef __PRNG_RANDMT64_H__
#define __PRNG_RANDMT64_H__

#include <stdint.h>

#define         RANDMT64_MAX UINT64_MAX

void            srandmt64(uint64_t seed);
void            srandmt64tab(uint64_t *key, uint64_t keylen);
uint64_t        randmt64(void);

#endif /* __PRNG_RANDMT64_H__ */

