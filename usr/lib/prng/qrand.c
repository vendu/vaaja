/*
 * this stuff is revamped from what's at
 * https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c
 */

#include <stdint.h>

static int32_t g_qrandseed = ~0;

static INLINE int32_t
qrand32_r(int32_t *seed)
{
    int32_t val = *seed;

    val *= 69069;
    val++;
    *seed = val;

    return val;
}

int32_t
qrand32(void)
{
    int32_t val = qrand32_r(&g_qrandseed);

    return val;
}

float qrandf(int32_t *seed)
{
    float val = qrand32_r(seed);

    return val;
}

int
Q_rand_r(int *seed) {
    *seed = (69069 * *seed + 1);

    return *seed;
}

int
Q_rand(void)
{
    int val = Q_rand_r(&g_qrandseed);

    return val;
}

