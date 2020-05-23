#include <stdint.h>
#include <zero/cdefs.h>

/* [always] inlined versions of zen hash functions */

/* the following two snippets were posted on stackoverflow by Thomas Mueller */

static C_INLINE uint32_t
tmhash32(uint32_t u)
{
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = (u >> 16) ^ u;

    return u;
}

static C_INLINE uint32_t
tmunhash32(uint32_t u)
{
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = (u >> 16) ^ u;

    return u;
}

static C_INLINE uint64_t
tmhash64(uint64_t u)
{
    u = (u ^ (u >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    u = (u ^ (u >> 27)) * UINT64_C(0x94d049bb133111eb);
    u = u ^ (u >> 31);

    return u;
}

static C_INLINE uint32_t
tmunhash64(uint32_t u)
{
    u = ((u >> 30) ^ u) * UINT64_C(0xbf58476d1ce4e5b9);
    u = ((u >> 27) ^ u) * UINT64_C(0x94d049bb133111eb);
    u = (u >> 31) ^ u;

    return u;
}

