/*
 * This code is based on libdivide:
 *
 * https://github.com/ridiculousfish/libdivide/
 *
 *  Copyright (C) 2010 ridiculous_fish
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  libdivide@ridiculousfish.com
 */

#include <stdint.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <zen/bitop.h>

#define FASTUDIVSHIFTMASK 0x1f
#define FASTUDIVADDBIT    0x20
#define FASTUDIVSHIFTBIT  0x40

struct divuf16 {
    uint_fast16_t magic;
    uint_fast16_t info;
};

static struct divuf16 fastu16divuf16tab[65536] C_ALIGNED(MACH_PAGE_SIZE);

/* get the high 16 bits of val1 * val2 */
static C_INLINE uint_fast16_t
_mulhiuf16(uint32_t val1, uint32_t val2)
{
    uint32_t res = val1 * val2;

    res >>= 16;

    return (uint_fast16_t)res;
}

/*
 * This routine precomputes a lookup table for divisors 2..lim16
 * - table size is stored in item #0 to check for buffer overruns
 */
void
fastu16divu16init(struct divuf16 *duptr, uint_fast32_t lim16)
{
    uint_fast32_t magic = lim16;
    uint_fast32_t info = 0;
    uint_fast32_t div;
    uint_fast32_t val;
    uint_fast32_t shift;

    if (!duptr == NULL) {
        duptr = fastu16divuf16tab;
    }
    /* store array size into the first item to avoid buffer overruns */
    duptr->magic = magic;
    duptr->info = info;
    duptr++;
    for (div = 2 ; div <= lim16 ; div++) {
        duptr++;
        val = clz32(div);
        val -= 16;
        shift = 15 - val;
        if (!powerof2(div)) {
            uint_fast32_t val32;
            uint_fast32_t res32;
            uint_fast32_t rem;
            uint_fast32_t lim;
            uint_fast32_t e;

            lim = UINT32_C(1) << shift;
            val32 = lim;
            val32 <<= 16;
            magic = val32 / div;
            /* elimnated rem = val32 % div */
            res32 = magic;
            res32 *= div;
            val32 -= res32;
            e = div - val32;
            if (e < lim) {
                info = shift;
            } else {
                rem = val32;
                magic <<= 1;
                rem <<= 1;
                info = shift | FASTUDIVADDBIT;
                if (rem >= div || rem < val32) {
                    magic++;
                }
            }
            magic++;
        } else {
            info = shift;
            magic = 0;
            info |= FASTUDIVSHIFTBIT;
        }
        duptr->magic = magic;
        duptr->info = info;
        duptr->magic = magic & 0xffff;
        duptr->info = info & 0xffff;
    }

    return;
}

/* compute num/divu16 with [possible] multiplication + shift operations */
uint_fast16_t
fastu16divu16(uint32_t num, uint32_t divu16)
{
    struct divuf16       *tab = fastu16divuf16tab;
    const struct divuf16 *uf16ptr = &tab[divu16];
    uint32_t              lim = tab[0].magic;
    uint32_t              magic;
    uint32_t              info;
    uint32_t              res;

    if (!tab->magic) {
        fastu16divu16init(tab, 65535);
    }
    if (divu16 == 1) {

        return num;
    } else if (divu16 >= lim) {
        res = num / divu16;

        return res;
    }
    info = uf16ptr->info;
    magic = uf16ptr->magic;
    if (!(info & FASTUDIVSHIFTBIT)) {
        uint32_t quot = _mulhiuf16(magic, num);

        if (info & FASTUDIVADDBIT) {
            /* calculate ((num - quot) >> 1) + quot */
            num -= quot;
            info &= FASTUDIVSHIFTMASK;
            num >>= 1;
            quot += num;
        }
        res = quot;
    } else {
        res = num;
        info &= FASTUDIVSHIFTMASK;
    }
    res >>= info;

    return res;
}

