#ifndef __PRNG_RANDKLC_H__
#define __PRNG_RANDKLC_H__

#include <mach/param.h>

/*
 * inline macros for Knuth's linear congruential pseudo-random number generator
 */

#define i_srandklc32(seedu32) (g_seed32 = (seed))
#define i_randklc32()                                                   \
    ((val) = g_seed32, (val) *= 69069, (val) += 1234567, g_seed32 = (val))

#if (LONGSIZE >= 8)
#define i_srandklc64(seedu64) (g_seed64 = (seed))
#define i_randklc64()                                                  \
    ((val) = g_seed64, (val) *= 6364136223846793005,                   \
     (val) += 1442695040888963407, g_seed64 = (val))
#endif

#endif /* __PRNG_RANDKLC_H__ */

