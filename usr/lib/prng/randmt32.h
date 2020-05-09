/*
 * 32-bit implementation of the Mersenne Twister MT19937 algorithm
 */

#ifndef __PRNG_RANDMT32_H__
#define __PRNG_RANDMT32_H__

#include <stdint.h>

#define RANDMT32_MAX UINT32_MAX

void            srandmt32(unsigned long seed);
unsigned long   randmt32(void);

#endif /* __PRNG_RANDMT32_H__ */

