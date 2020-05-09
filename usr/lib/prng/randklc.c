/* linear congruential random number generator from Donald Knuth */

#include <stdint.h>
#include <zero/randklc.h>

static uint32_t g_seed64;
static uint32_t g_seed32;

void
srandklc32(uint32_t seed)
{
    i_srandklc32(seed);

    return;
}

uint32_t
randklc32(void)
{
    uint32_t val = i_randklc32();

    return val;
}

void
srandklc64(uint64_t seed)
{
    i_srandklc64(seed);

    return;
}

uint64_t
randklc64(void)
{
    uint64_t val = i_randklc64();

    return val;
}

