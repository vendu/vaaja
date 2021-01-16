#include <prng/randmt32.h>

void
srand(unsigned int seed)
{
    srandmt32(seed);

    return;
}

int
rand(void)
{
    int retval = (int)randmt32();

    return retval;
}

