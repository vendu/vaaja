#include <stdint.h>
#include <zero/cdefs.h>
#if (TEST_OP)
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#endif

/* count bits, check parity, etc. one byte at a time from LUTs */
#define V0_VM_BIT_TAB_ITEMS 256

static uint8_t  v0bitclztab[V0_VM_BIT_TAB_ITEMS];
static uint8_t  v0bitctztab[V0_VM_BIT_TAB_ITEMS];
static uint8_t  v0bitpartab[V0_VM_BIT_TAB_ITEMS];

static void
v0initops(void)
{
    int i;

    for (i = 0 ; i < V0_VM_BIT_TAB_ITEMS ; i++) {
        v0bitclztab[i] = __builtin_clz(i) - 24;
        v0bitctztab[i] = __builtin_ctz(i);
        v0bitpartab[i] = __builtin_parity(i);
    }

    return;
}

static C_INLINE int8_t
m_clz32lut(uint32_t u32)
{
    uint32_t    ub1 = (u32 >> 24) & 0xff;
    uint32_t    ub2 = (u32 >> 16) & 0xff;
    uint32_t    ub3 = (u32 >> 8) & 0xff;
    uint32_t    ub4 = u32 & 0xff;
    int8_t      res;

    res = 0;
    if (u32) {
        if (!ub1) {
            res += 8;
            if (!ub2) {
                res += 8;
                if (!ub3) {
                    res += 8;
                    if (!ub4) {
                        res += 8;
                    } else {
                        res += v0bitclztab[ub4];
                    }
                } else {
                    res += v0bitclztab[ub3];
                }
            } else {
                res += v0bitclztab[ub2];
            }
        } else {
            res += v0bitclztab[ub1];
        }
    } else {

        return 32;
    }

    return res;
}

static C_INLINE int8_t
m_ctz32lut(uint32_t u32)
{
    uint32_t    ub1 = (u32 >> 24) & 0xff;
    uint32_t    ub2 = (u32 >> 16) & 0xff;
    uint32_t    ub3 = (u32 >> 8) & 0xff;
    uint32_t    ub4 = u32 & 0xff;
    int8_t      res;

    res = 0;
    if (u32) {
        if (!ub4) {
            res += 8;
            if (!ub3) {
                res += 8;
                if (!ub2) {
                    res += 8;
                    if (!ub1) {
                        res += 8;
                    } else {
                        res += v0bitctztab[ub1];
                    }
                } else {
                    res += v0bitctztab[ub2];
                }
            } else {
                res += v0bitctztab[ub3];
            }
        } else {
            res += v0bitctztab[ub4];
        }
    } else {

        return 32;
    }

    return res;
}

static C_INLINE int8_t
m_par32lut(uint32_t u32)
{
    uint32_t    ub1 = (u32 >> 24) & 0xff;
    uint32_t    ub2 = (u32 >> 16) & 0xff;
    uint32_t    ub3 = (u32 >> 8) & 0xff;
    uint32_t    ub4 = u32 & 0xff;
    int8_t      res;

    res = v0bitpartab[ub1];
    res += v0bitpartab[ub2];
    res += v0bitpartab[ub3];
    res += v0bitpartab[ub4];
    res &= 0x01;

    return res;
}

#if (TEST_OP)
int
main(int argc, char *argv[])
{
    int i;
    int j;

    v0initops();
    for (i = 0 ; i < INT_MAX ; i++) {
        j = m_clz32lut(i);
        if (j != __builtin_clz(i)) {
            printf("CLZ(%x) => %d (%d)\n", i, j, __builtin_clz(i));
        }
        j = m_ctz32lut(i);
        if (j != __builtin_ctz(i)) {
            printf("CTZ(%x) => %d (%d)\n", i, j, __builtin_clz(i));
        }
        j = m_par32lut(i);
        if (j != __builtin_parity(i)) {
            printf("CTZ(%x) => %d (%d)\n", i, j, __builtin_parity(i));
        }
    }
    j = m_clz32lut(i);
    if (j != __builtin_clz(i)) {
        printf("CLZ(%x) != %d\n", __builtin_clz(i), j);
    }
    j = m_ctz32lut(i);
    if (j != __builtin_ctz(i)) {
        printf("CTZ(%x) != %d\n", __builtin_clz(i), j);
    }
}
#endif /* (TEST_OP) */

