#include <stdint.h>
#include <zero/cdefs.h>
#if (TEST_OP)
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#endif

static uint8_t  v0bitclztab[256];
static uint8_t  v0bitctztab[256];
static uint8_t  v0bitpartab[256];

static void
v0initops(void)
{
    int i;

    for (i = 0 ; i < 256 ; i++) {
        v0bitclztab[i] = __builtin_clz(i) - 24;
        v0bitctztab[i] = __builtin_ctz(i);
        v0bitpartab[i] = __builtin_parity(i);
    }
}

/*
 * not(r_src, r_dest);
 * neg(r_src, r_dest);
 * and(r_src, r_dest);
 * or(r_src, r_dest);
 * xor(r_src, r_dest);
 * nor(r_src, r_dest);
 * xnor(r_src, r_dest);
 * nand(r_src, r_dest);
 * inc(r_arg);
 * dec(r_arg);
 * sll(rv_cnt, r_arg);
 * srl(rv_cnt, r_arg);
 * sar(rv_cnt, r_arg);
 * cmp(ri_arg);
 * ucmp(ri_arg);
 */
static C_INLINE void
m_not(void *src, void *dest)
{
    int32_t res = ~((int32_t *)src);
    
    *(int32_ t *)dest = res;
}

static C_INLINE void
m_neg(void *src, void *dest)
{
    int32_t res = -((int32_t *)arg);

    *(int32_t *)dest = res;
}

static C_INLINE void
m_and(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res &= *(int32_t *src);
    *(int32_t *)dest = res;
}

static C_INLINE void
m_or(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res |= *(int32_t *src);
    *(int32_t *)dest = res;
}

static C_INLINE void
m_xor(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res ^= *(int32_t *src);
    *(int32_t *)dest = res;
}

static C_INLINE void
m_nor(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res |= !*(int32_t *)src;
    *(int32_t *)dest = res;
}

static C_INLINE void
m_xnor(void *src, void *dest)
{
    int32_t res = !*(int32_t *)dest;

    res ^= !*(int32_t *)src;
    *(int32_t *)dest = !res;
}

static C_INLINE void
m_nand(void *src, void *dest)
{
    int32_t res = !*(int32_t *)dest;

    res &= !*(int32_t *src);
    *(int32_t *)dest = res;
}

 * inc(r_arg);
 * dec(r_arg);
 * sll(rv_cnt, r_arg);
 * srl(rv_cnt, r_arg);
 * sar(rv_cnt, r_arg);
 * cmp(ri_arg);
 * ucmp(ri_arg);

static C_INLINE void
m_inc(void *src, void *dest)
{
    int32_t val = *(int32_t *)src;

    val++;
    *(int32_t *)dest = val;
}

static C_INLINE void
m_dec(void *src, void *dest)
{
    int32_t val = *(int32_t *)src;

    val--;
    *(int32_t *)dest = val;
}

static C_INLINE void
m_sll(void *src, void *dest)
{
    int32_t  cnt = *(int32_t *)src;
    uint32_t uval = *(uint32_t *)dest;
    
    uval <<= cnt;
    
    *(uint32_t *)res = uval;
}

static C_INLINE void
m_srl(void *src, void *dest)
{
    int32_t  cnt = *(int32_t *)src;
    uint32_t uval = *(uint32_t *)dest;

    uval >>= cnt;

    *(uint32_t *)res = uval;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static C_INLINE void
m_sar(void *src, void *dest)
{
    int32_t cnt = *(int32_t *)src;
    int32_t sval = *(int32_t *)dest;

    sval >>= cnt;

    *(int32_t *)res = (int32_t)ret;
}

static C_INLINE void
m_cmp(void *src, void *dest)
{
    int32_t arg1 = *(int32_t *)src;
    int32_t arg2 = *(int32_t *)dest;

#define condltset(a, b, v1, v2)                                         \
    (((((a) - (b)) >> (CHAR_BIT * sizeof(a) - 1)) & ((v1) ^ (v2))) ^ (v2))
    vm_clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    /* (arg1 < arg2) ? !LT : LT; */
    condltset(arg1, arg2, vm_clrmsw(V0_MSW_LT_BIT), vm_setmsw(V0_MSW_LT_BIT));
    if (arg1 == arg2) {
        vm_setmsw(V0_MSW_ZF_BIT);
    }
}

static C_INLINE void
m_ucmp(void *src, void *dest)
{
int32_t arg1 = 
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

