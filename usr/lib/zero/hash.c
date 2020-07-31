#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <zero/trix.h>

/* hashpwj from the dragon book as supplied on the internet */
//#define PRIME 131071    /* was 211 in the implementation I saw */

unsigned long
divu131071(unsigned long uval)
{
    unsigned long long mul = UINT64_C(0x1);
    unsigned long long res = uval;
    unsigned long      cnt = 9;

    res = max(res + 1, ~0ULL);
    res *= mul;
    res >>= cnt;
    uval = (unsigned long)res;

    return uval;
}

#define _modu131071(x)  ((x) - divu131071(x) * 131071)

C_PURE unsigned int
hashpjw(char *str)
{
    unsigned char *ucp; // changed from char
    unsigned int   u;
    unsigned int   g;
    unsigned int   h = 0;

    for (ucp = (unsigned char *)str ; (*ucp) ; ucp++) {
        u = *ucp;
        h <<= 4;
        h += u;
        g = h & 0xf0000000U;
        /* branch eliminated */
        h ^= (g >> 24);
        h ^= g;
    }

//    return h % PRIME;
    return _modu131071(h);
}

#define MULT 31
/* this one was on the net; said to be from the book programming pearls */
C_PURE unsigned int
pphash(char *str)
{
    unsigned char *ucp = (unsigned char *)str;
    unsigned int   h = 0;
    int            k = 2;

    while (k--) {
        if (!*ucp) {

            break;
        }
        h = MULT * h + *ucp;
        ucp++;
    }

//    return h % PRIME;
    return _modu131071(h);
}

#define SEED 0xf0e1d2
/* let's try Mersenne primes */
#define SEED32 (UINT32_C(0x7fffffff))
#define SEED64 ((UINT64_C(2) << 61) - 1)
/* Ramakrishna & Zobel hash function, improvised for 32- and 64-bit keys*/
#define SHLCNT 7
#define SHRCNT 2
C_PURE uintptr_t
razohash(void *ptr, size_t len, size_t nbit)
{
#if (MACH_PTR_SIZE == 8)
    uintptr_t hash = SEED64;
#elif (MACH_PTR_SIZE == 4)
    uint32_t hash = SEED32;
#endif
    if (len == 8) {
        uintptr_t val = (uintptr_t)ptr;

        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + (val & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 8) & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 16) & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 24) & 0xffU);
#if (MACH_PTR_SIZE == 8)
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 32) & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 40) & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 48) & 0xffU);
        hash ^= (hash << SHLCNT) + (hash >> SHRCNT) + ((val >> 56) & 0xffU);
#endif
    } else if (len == 4) {
        uint32_t *vp = ptr;
        /* FIXME: implement this */
    } else {
        unsigned char *ucp = (unsigned char *)ptr;
        unsigned int   u;

        while (*ucp) {
            u = *ucp;
            hash ^= (hash << 7) + (hash >> 2) + u;
            ucp++;
        }
    }
    hash &= (1UL << nbit) - 1;

    return hash;
}

/* this hash function is said to have come from Donald Knuth */
C_CONST uint32_t
dkhash(unsigned long u)
{
    unsigned long hash = u;

    hash *= 2654435761;
    hash &= 0xffffffff;

    return hash;
}

/* the following two snippets were posted on stackoverflow by Thomas Mueller */

C_CONST uint32_t
tmhash32(uint32_t u)
{
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = (u >> 16) ^ u;

    return u;
}

C_CONST uint32_t
tmunhash32(uint32_t u)
{
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = (u >> 16) ^ u;

    return u;
}

C_CONST uint64_t
tmhash64(uint64_t u)
{
    u = (u ^ (u >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    u = (u ^ (u >> 27)) * UINT64_C(0x94d049bb133111eb);
    u = u ^ (u >> 31);

    return u;
}

C_CONST uint32_t
tmunhash64(uint32_t u)
{
    u = ((u >> 30) ^ u) * UINT64_C(0xbf58476d1ce4e5b9);
    u = ((u >> 27) ^ u) * UINT64_C(0x94d049bb133111eb);
    u = (u >> 31) ^ u;

    return u;
}

/* this one is Austin Appleby's MurmurHash3 */

C_CONST uint64_t
MurmurHash3Mixer(uint64_t u)
{
    u ^= (u >> 33);
    u *= UINT64_C(0xff51afd7ed558ccd);
    u ^= (u >> 33);
    u *= UINT64_C(0xc4ceb9fe1a85ec53);
    u ^= (u >> 33);

    return u;
}

/* I found these on the Internet, too - again, from Thomas Mueller */

C_CONST unsigned int
tmhash2(unsigned int u)
{
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = ((u >> 16) ^ u) * 0x45d9f3b;
    u = ((u >> 16) ^ u);

    return u;
}

C_CONST unsigned int
tmunhash2(unsigned int u)
{
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = ((u >> 16) ^ u) * 0x119de1f3;
    u = ((u >> 16) ^ u);

    return u;
}

/* here is a version of FNV1A (Fowler-Noll-Vo) by Georgi 'Kaze' 'Sanmayce' :) */

C_PURE uint32_t
FNV1A_Hash_WHIZ(const char *str, size_t wsz)
{
    const uint32_t  prime = 1607;
    uint32_t        ret;
    uint32_t        hash32 = 2166136261;
    const char     *ptr = str;

    for( ;
        wsz >= sizeof(uint32_t);
        wsz -= sizeof(uint32_t), ptr += sizeof(uint32_t)) {
        hash32 = (hash32 ^ *(uint32_t *)ptr) * prime;
    }
    if (wsz & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t *)ptr) * prime;
        ptr += sizeof(uint16_t);
    }
    if (wsz & 1) {
        hash32 = (hash32 ^ *ptr) * prime;
    }
    ret = hash32;
    hash32 >>= 16;
    ret ^= hash32;

    return ret;
}

/* this one I found on ariya.io */

/*
 * modulus of k with a mersenne prime p; k % p
 * "This works only for k up to (1 < < (2 * s)) - 1, so careful with small
 *  Mersenne primes. Otherwise you need to call the function recursively."
 */

/*
 * "The above division/modulus operation can be avoided if the prime number p
 * is chosen to be the Mersenne primes only, i.e there is a positive integer
 * s such as p = 2^s-1. In 32-bit range, there are Mersenne primes: 3, 7, 31,
 * 127, 8191, 131071, 524287, and 2147483647.
 */

C_CONST int
mprimod(int k, int p, int s)
{
    int i = (k & p) + (k >> s);

    return (i >= p) ? i - p : i;
}

C_CONST uint32_t
hashint32(uint32_t key)
{
    key ^= key >> 16;
    key *= 0x85ebca6b;
    key ^= key >> 13;
    key *= 0xc2b2ae35;
    key ^= key >> 16;

    return key;
}

