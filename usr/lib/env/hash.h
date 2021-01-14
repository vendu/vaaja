#ifndef __ZERO_HASH_H__
#define __ZERO_HASH_H__

#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#if defined(_REENTRANT)
#include <mt/mtx.h>
#define hashlk(lp)   mtxlk(lp)
#define hashunlk(lp) mtxunlk(lp)
#else
#define hashlk(lp)
#define hashunlk(lp)
#endif
#include <prng/randmt32.h>

/* #define HASH_FUNC    - takes one parameter of HASH_TYPE * */
/* #define HASH_CMP     - takes two parameters of HASH_TYPE *; 0 if equal */
/* #define HASH_TYPE    - hash table item type */
/* #define HASH_KEYTYPE - hash key type (such as long) */

/*
 * Example
 * -------
 *
 * struct hash {
 * #if (_REENTRANT)
 *     volatile long  lk;
 * #endif
 *     HASH_TYPE     *tab;
 * };
 *
 * typedef long HASH_KEYTYPE;
 *
 * typedef struct {
 *     HASH_KEYTYPE  key;
 *     HASH_TYPE    *prev;
 *     HASH_TYPE    *next;
 * } HASH_TYPE;
 *
 * #define HASH_TABSZ         65536
 * #define HASH_FUNC(ip)      ((ip)->key & 0xffff)
 * #define HASH_CMP(ip1, ip2) (!((ip1)->key == (ip2)->key))
 * #define HASH_ALLOC(n, sz)  calloc(n, sz)
 */

#if !defined(HASH_ALLOC)
#define HASH_ALLOC(n, sz)     calloc(n, sz)
#endif

#define hashinit()                                                      \
    do {                                                                \
        HASH_TYPE *_hash = HASH_ALLOC(1, sizeof(HASH_TYPE));            \
                                                                        \
        if (_hash) {                                                    \
            _hash->tab = HASH_ALLOC(HASH_TABSZ, sizeof(HASH_TYPE));     \
        }                                                               \
    } while (0)

#define hashadd(hash, item)                                             \
    do {                                                                \
        HASH_KEYTYPE  _key = HASH_FUNC(item);                           \
        HASH_TYPE    *_item;                                            \
                                                                        \
        (item)->HASHPREV = NULL;                                        \
        hashlk(&(hash)->lk);                                            \
        _item = (hash)->tab[_key];                                      \
        if (_item) {                                                    \
            _item->HASHPREV = item;                                     \
        }                                                               \
        (item)->HASHNEXT = _item;                                       \
        (hash)->tab[_key] = item;                                       \
        hashunlk(&(hash)->lk);                                          \
    } while (0)

#define hashfind(hash, item, rpp)                                       \
        _hashsrch(hash, item, rpp, 0)

#define hashrm(hash, item, rpp)                                         \
        _hashsrch(hash, item, rpp, 1)

#define _hashsrch(hash, item, rpp, rm)                                  \
    do {                                                                \
        HASH_KEYTYPE  _key = HASH_FUNC(item);                           \
        HASH_TYPE    *_item = NULL;                                     \
        HASH_TYPE    *_item1;                                           \
        HASH_TYPE    *_item2;                                           \
                                                                        \
        hashlk(&(hash)->lk);                                            \
        _item = (hash)->tab[key];                                       \
        while (_item) {                                                 \
            if (!HASH_CMP(_item, item)) {                               \
                if (rm) {                                               \
                    _item1 = _item->HASHPREV;                           \
                    _item2 = _item->HASHNEXT;                           \
                    if (_item1) {                                       \
                        _item1->HASHNEXT = _item2;                      \
                    }                                                   \
                    if (_item2) {                                       \
                        _item2->HASHPREV = _item1;                      \
                    }                                                   \
                }                                                       \
                                                                        \
                break;                                                  \
            }                                                           \
            _item = _item->HASHNEXT;                                    \
        }                                                               \
        hashunlk(&(hash)->lk);                                          \
        *(rpp) = _item;                                                 \
    } while (FALSE)

#if (LONGSIZE == 8) || (LONGLONGSIZE == 8)
uint64_t
#elif (LONGSIZE == 4)
uint32_t
#endif
razohash(void *ptr, size_t len, size_t nbit);

uint32_t tmhash32(unsigned long u);
uint64_t tmhash64(uint64_t u);

/*
 * fanom32 - FAst NOn-Multiplicative 32-bit hash
 */

/*
 * Copyright (C) 2016 Sokolov Yura aka funny_falcon
 *
 * It is in public domain, free to use and modify by everyone for any usage.
 * I will be glad, if you put copyright notice above in your code or distribution, but you don't ought to.
 * I'm not responsible for any damage cause by this piece of code.
 *
 * But if you like licenses, than you may count it is under MIT Licence.
 */
/*
 * Copyright (C) 2014 Sokolov Yura aka funny_falcon
 *
 * It is in public domain, free to use and modify by everyone for any usage.
 * I will be glad, if you put copyright notice above in your code or distribution, but you don't ought to.
 * I'm not responsible for any damage cause by this piece of code.
 */

/* TODO: inline asm, tmp-variable */
#define __hashrotu32(u32, cnt)                                          \
    (((u32) << (cnt)) | ((u32) >> (sizeof(uint32_t) * CHAR_BIT - (cnt))))

static INLINE uint32_t
__hashloadu24(const uint8_t *buf, size_t len)
{
    uint32_t tmp = buf[len >> 1];
    uint32_t u32 = buf[0];

    tmp <<= 8;
    u32 |= tmp;
    tmp = buf[len - 1];
    tmp <<= 16;
    u32 |= tmp;

    return u32;
}

#define __hashloadu32(buf) __hashloadu32lsb(buf)

/*
 * dropped unaligned read support in favor of byte-by-byte reading which should
 * be relatively fast
 */
static INLINE uint32_t
__hashloadu32lsb(const uint8_t *buf)
{
    const uint8_t *u8ptr = buf;
    uint32_t       u32 = u8ptr[0];
    uint32_t       tmp = u8ptr[1];

    tmp <<= 8;
    u32 |= tmp;
    tmp = u8ptr[2];
    tmp <<= 16;
    u32 |= tmp;
    tmp = u8ptr[3];
    tmp <<= 24;
    u32 |= tmp;

    return u32;
}

static INLINE uint32_t
__fanom32permstr(const uint8_t *str, size_t len, uint32_t seed1, uint32_t seed2)
{
        uint32_t a = 0;
        uint32_t b = 0;
        uint32_t c = seed1;
        uint32_t d = seed2;
        uint32_t l = len;
        uint32_t val = 0;

        switch (len) {
            case 0:

                break;
            case 1:
            case 2:
            case 3:
                a = val = __hashloadu24(str, len);

                break;
        case 4:
                a = val = __hashloadu32(str);

                break;
        case 5: case 6: case 7:
                a = __hashloadu32(str);
                b = __hashloadu24(str + 4, len & 3);

                break;
        case 8:
                a = __hashloadu32(str);
                b = __hashloadu32(str + 4);

                break;
        default:
                for(; len > 8; len -= 8, str += 8) {
                        a ^= __hashloadu32(str);
                        b ^= __hashloadu32(str + 4);
                        c += a;
                        d += b;
                        a = __hashrotu32(a, 5) - d;
                        b = __hashrotu32(b, 7) - c;
                        c = __hashrotu32(c, 24) ^ a;
                        d = __hashrotu32(d, 1) ^ b;
                }
                a ^= __hashloadu32(str + len - 8);
                b ^= __hashloadu32(str + len - 4);
        }
        c += b; c -= __hashrotu32(a, 9);
        d += a; d -= __hashrotu32(b, 18);
        val = seed2 ^ l;
        a ^= b;
        val -= __hashrotu32(a, 7);
        val += c;
        val += __hashrotu32(d, 13);
        d ^= c;
        d -= __hashrotu32(c, 25);
        val ^= d;
        val -= __hashrotu32(d, 16);
        c ^= val;
        c -= __hashrotu32(val, 4);
        d ^= c;
        d -= __hashrotu32(c, 14);
        val ^= d;
        val -= __hashrotu32(d, 24);

        return val;
}

#define FANOM32_HASH_SEED 0xeb0d2f41U

static INLINE uint32_t
fanom32hashstr(const void *ptr, size_t len, uint32_t seed)
{
    uint32_t       val = seed;

    val ^= FANOM32_HASH_SEED;
    val = __fanom32permstr(ptr, len, seed, val);

    return val;
}

static INLINE uint32_t
fanom32hashstr2(const void *ptr, size_t len, uint32_t seed1, uint32_t seed2)
{
    uint32_t u32 = __fanom32permstr(ptr, len, seed1, seed2);

    return u32;
}

#endif /* __ZERO_HASH_H__ */

