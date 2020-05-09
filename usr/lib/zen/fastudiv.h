#ifndef __ZERO_FASTUDIV_H__
#define __ZERO_FASTUDIV_H__

#include <stdio.h>
#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#if defined(__KERNEL__)
#include <kern/util.h>
#include <kern/cpu.h>
#include <kern/unit/x86/trap.h>
#else /* !defined(__KERNEL__) */
#include <stdlib.h>
#endif

#define FASTUF16DIVUF16     1
#define FASTUF16DIVUF32     1

#define FASTU32DIVSHIFTMASK 0x1f
#define FASTU32DIVADDBIT    0x20
#define FASTU32DIVSHIFTBIT  0x40

struct divu16 {
    uint16_t magic;
    uint16_t info;
};

struct divu32 {
    uint32_t magic;
    uint32_t info;
};

#if (FASTUF16DIVUF16)

struct divuf16 {
    uint_fast16_t magic;
    uint_fast16_t info;
};

void fastuf16divuf16gentab(struct divuf16 *duptr, uint_fast32_t lim16);

#endif

#if (FASTUF16DIVUF32)

struct divuf32 {
    uint_fast32_t magic;
    uint_fast32_t info;
};

void fastuf16divuf32gentab(struct divuf32 *duptr, uint_fast32_t lim16);

#endif

void fastu16divu16gentab(struct divu16 *duptr, uint32_t lim16);

#if (FASTUF16DIVUF16)

/* get the high 16 bits of val1 * val2 */
static INLINE uint_fast16_t
_mulhiuf16(uint32_t val1, uint32_t val2)
{
    uint32_t res = val1 * val2;

    res >>= 16;

//    return res & 0xffff;
    return (uint_fast16_t)res;
}

#endif

#if (FASTUF16DIVUF32)

/* get the high 16 bits of val1 * val2 */
static INLINE uint_fast16_t
_mulhiuf32(uint_fast32_t val1, uint_fast32_t val2)
{
    uint_fast32_t res = val1 * val2;

    res >>= 16;

    return res;
}

#endif

/* get the high 16 bits of val1 * val2 */
static INLINE uint16_t
_mulhiu16(uint32_t val1, uint32_t val2)
{
    uint32_t res = val1 * val2;

    res >>= 16;

    return (uint16_t)res;
}

#if (FASTUF16DIVUF16)

/* compute num/divu16 with [possible] multiplication + shift operations */
static INLINE uint_fast16_t
fastuf16divuf16(uint_fast16_t num, uint_fast16_t divuf16,
                const struct divuf16 *tab)
{
    const struct divuf16 *ulptr = &tab[divuf16];
    uint32_t              lim = tab[0].magic;
    uint32_t              magic;
    uint32_t              info;
    uint32_t              res;

    if (divuf16 == 1) {

        return num;
    } else if (divuf16 >= lim) {
        res = num / divuf16;

        return res;
    }
    info = ulptr->info;
    magic = ulptr->magic;
    if (!(info & FASTU32DIVSHIFTBIT)) {
        uint32_t quot = _mulhiuf16(magic, num);

        if (info & FASTU32DIVADDBIT) {
            /* calculate ((num - quot) >> 1) + quot */
            num -= quot;
            info &= FASTU32DIVSHIFTMASK;
            num >>= 1;
            quot += num;
        }
        res = quot;
    } else {
        res = num;
        info &= FASTU32DIVSHIFTMASK;
    }
    res >>= info;

    return res;
}

#endif

#if (FASTUF16DIVUF32)

/* compute num/divu16 with [possible] multiplication + shift operations */
static INLINE uint_fast16_t
fastuf16divuf32(uint_fast16_t num, uint_fast16_t divuf16,
                const struct divuf32 *tab)
{
    const struct divuf32 *ulptr = &tab[divuf16];
    uint32_t              lim = tab[0].magic;
    uint32_t              magic;
    uint32_t              info;
    uint32_t              res;

    if (divuf16 == 1) {

        return num;
    } else if (divuf16 >= lim) {
        res = num / divuf16;

        return res;
    }
    info = ulptr->info;
    magic = ulptr->magic;
    if (!(info & FASTU32DIVSHIFTBIT)) {
        uint32_t quot = _mulhiuf16(magic, num);

        if (info & FASTU32DIVADDBIT) {
            /* calculate ((num - quot) >> 1) + quot */
            num -= quot;
            info &= FASTU32DIVSHIFTMASK;
            num >>= 1;
            quot += num;
        }
        res = quot;
    } else {
        res = num;
        info &= FASTU32DIVSHIFTMASK;
    }
    res >>= info;

    return res;
}

#endif

/* compute num/divu16 with [possible] multiplication + shift operations */
static INLINE uint16_t
fastu16divu16(uint16_t num, uint16_t divu16,
              const struct divu16 *tab)
{
    const struct divu16 *ulptr = &tab[divu16];
    uint32_t             lim = tab[0].magic;
    uint32_t             magic;
    uint32_t             info;
    uint32_t             res;

    if (divu16 == 1) {

        return num;
    } else if (divu16 >= lim) {
        res = num / divu16;

        return res;
    }
    info = ulptr->info;
    magic = ulptr->magic;
    if (!(info & FASTU32DIVSHIFTBIT)) {
        uint32_t quot = _mulhiu16(magic, num);

        if (info & FASTU32DIVADDBIT) {
            /* calculate ((num - quot) >> 1) + quot */
            num -= quot;
            info &= FASTU32DIVSHIFTMASK;
            num >>= 1;
            quot += num;
        }
        res = quot;
    } else {
        res = num;
        info &= FASTU32DIVSHIFTMASK;
    }
    res >>= info;

    return res;
}

#endif /* __ZERO_FASTUDIV_H__ */

