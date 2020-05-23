#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mt/mtx.h>
//#include <zero/trix.h>
//#include <v0/mach32.h>
#include <v0/types.h>
#include <v0/vm.h>
#include <v0/regs.h>
#include <v0/isa.h>
#include <v0/bsp/papilio-pro.h>

/* count bits, check parity, etc. one byte at a time from LUTs */
#define V0_VM_BIT_TAB_ITEMS 256
static uint8_t  v0bitclztab[V0_VM_BIT_TAB_ITEMS];
static uint8_t  v0bitctztab[V0_VM_BIT_TAB_ITEMS];
static uint8_t  v0bitpartab[V0_VM_BIT_TAB_ITEMS];

struct vm g_vm C_ALIGNED(MACH_CL_SIZE);

static void
v0initbittabs(void)
{
    int i;

    for (i = 0 ; i < V0_VM_BIT_TAB_ITEMS ; i++) {
        v0bitclztab[i] = __builtin_clz(i) - 24;
        v0bitctztab[i] = __builtin_ctz(i);
        v0bitpartab[i] = __builtin_parity(i);
    }

    return;
}

struct vm *
v0initvm(struct vm *vm, int flg, size_t dramsize)
{
    void   *ptr;

    mtlkfmtx(&vm->mtx);
    if (flg & V0_VM_READY) {
        mtunlkfmtx(&vm->mtx);

        return vm;
    }
    ptr = calloc(dramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM memory...\n");
        abort();
    }
    vm->mem = ptr;
    vm->memsize = dramsize;
    vm->flg |= V0_VM_READY;
    v0initbittabs();
    mtunlkfmtx(&vm->mtx);

    return vm;
}

/* ALU-unit */

static C_INLINE void
v0_not(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = ~(*(int32_t *)src);
    
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_neg(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = -(*(int32_t *)src);

    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_and(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_or(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xor(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res ^= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_nor(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xnor(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res ^= !*(int32_t *)src;
    *(int32_t *)dest = !res;

    return;
}

static C_INLINE void
v0_nand(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = ~*(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

#if 0
static C_INLINE void
v0_nand(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res &= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}
#endif

static C_INLINE void
v0_inc(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val++;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_dec(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val--;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_sll(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t uval = *(uint32_t *)dest;
    int32_t  cnt = *(int32_t *)src;
    
    uval <<= cnt;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_srl(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t uval = *(uint32_t *)dest;
    int32_t  cnt = *(int32_t *)src;

    uval >>= cnt;
    *(uint32_t *)dest = uval;

    return;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static C_INLINE void
v0_sar(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t dval = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    dval >>= cnt;
    *(int32_t *)dest = (int32_t)dval;

    return;
}

static C_INLINE void
v0_rol(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int64_t d64 = *(int32_t *)dest;
    int32_t d32 = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    ;
}

v0_ror(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int64_t d64 = *(int32_t *)dest;
    int32_t d32 = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    ;
}

v0_rcl(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int64_t d64 = *(int32_t *)dest;
    int32_t d32 = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    ;
}

v0_rcr(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int64_t d64 = *(int32_t *)dest;
    int32_t d32 = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    ;
}

v0_add(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 += s32;
    *(int32_t *)dest = d32;

    return;
}

v0_addu(void *src, void *dest, C_UNUSED m_word_t parm)
{
    ;
}

v0_adc(void *src, void *dest, C_UNUSED m_word_t parm)
{
    ;
}

v0_sub(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 -= s32;
    *(int32_t *)dest = d32;

    return;
}

v0_sbc(void *src, void *dest, C_UNUSED m_word_t parm)
{
    ;
}

v0_mul(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 *= s32;
    *(int32_t *)dest = d32;

    return;
}

v0_umul(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t s32 = *(uint32_t *)src;
    uint32_t d32 = *(uint32_t *)dest;

    d32 *= s32;
    *(uint32_t *)dest = d32;

    return;
}

v0_mhi(void *src, void *dest, C_UNUSED m_word_t parm)
{
    ;
}

v0_umhi(void *src, void *dest, C_UNUSED m_word_t parm)
{
    ;
}

static C_INLINE void
v0_idiv(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 = irpdiv(d32, s32);
    *(int32_t *)dest = d32;

    return;
}

static C_INLINE void
v0_div(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 /= s32;

    *(int32_t *)dest = d32;

    return;
}

static C_INLINE void
v0_udiv(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    s32 = *(uint32_t *)src;
    uint32_t    d32 = *(uint32_t *)dest;

    d32 /= s32;

    *(uint32_t *)dest = d32;

    return;
}

static C_INLINE void
v0_rem(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 %= s32;

    *(int32_t *)dest = d32;

    return;
}

static C_INLINE void
v0_urem(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    s32 = *(uint32_t *)src;
    uint32_t    d32 = *(uint32_t *)dest;

    d32 %= s32;

    *(uint32_t *)dest = d32;

    return;
}

/* BIT-unit */

static C_INLINE void
m_zex(void *src, void *dest, m_word_t parm)
{
    int32_t val;

    switch (parm) {
        case 0:
            val = *(int8_t *)src;

            break;
        case 1:
            val = *(int16_t *)src;

            break;
        case 2:
            val = *(int32_t *)src;

            break;
        case 3:
            fprintf(stderr, "V0 does not support 32+-bit zero-extension...");

            abort();
    }
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
m_sex(void *src, void *dest, m_word_t parm)
{
    int32_t cnt = 8 << parm;
    int32_t sign = *(int32_t *)src & V0_SIGN_BIT;

    if (sign) {
        int32_t mask = ~INT32_C(0);
        int32_t tmp;
            
        mask <<= cnt;
        tmp = cnt - 1;
        *(int32_t *)dest = mask | tmp;
    }

    return;
}

static C_INLINE void
m_clz32lut(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    u32 = *(uint32_t *)src;
    uint32_t    ub1 = (u32 >> 24) & 0xff;
    uint32_t    ub2 = (u32 >> 16) & 0xff;
    uint32_t    ub3 = (u32 >> 8) & 0xff;
    uint32_t    ub4 = u32 & 0xff;
    int32_t     res;

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

        *(int32_t *)dest = 32;
    }
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
m_ctz32lut(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    u32 = *(uint32_t *)src;
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

        *(int32_t *)dest = 32;
    }
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
m_par32lut(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    u32 = *(uint32_t *)src;
    uint32_t    ub1 = (u32 >> 24) & 0xff;
    uint32_t    ub2 = (u32 >> 16) & 0xff;
    uint32_t    ub3 = (u32 >> 8) & 0xff;
    uint32_t    ub4 = u32 & 0xff;
    int32_t      res;

    res = v0bitpartab[ub1];
    res += v0bitpartab[ub2];
    res += v0bitpartab[ub3];
    res += v0bitpartab[ub4];
    res &= 0x01;
    *(int32_t *)dest = res;

    return;
}

/* SUBRT-unit */

static C_INLINE void
v0_cmp(void *src, void *dest, C_UNUSED m_word_t parm)
{
    int32_t arg2 = *(int32_t *)dest;
    int32_t arg1 = *(int32_t *)src;

    v0_clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    if (arg1 < arg2) {
        v0_setmsw(V0_MSW_LT_BIT);
    } else {
        v0_clrmsw(V0_MSW_LT_BIT);
    }
    if (arg1 == arg2) {
        v0_setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

static C_INLINE void
v0_ucmp(void *src, void *dest, C_UNUSED m_word_t parm)
{
    uint32_t arg2 = *(uint32_t *)dest;
    uint32_t arg1 = *(uint32_t *)src;

    v0_clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    if (arg1 < arg2) {
        v0_setmsw(V0_MSW_LT_BIT);
    } else {
        v0_clrmsw(V0_MSW_LT_BIT);
    }
    if (arg1 == arg2) {
        v0_setmsw(V0_MSW_ZF_BIT);
    }
    if (arg1 == arg2) {
        v0_setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

int
main(int argc, char *argv[])
{
    int i;
    int j;

    v0initvm(&g_vm, 0, PAPILIO_PRO_DRAM_SIZE);

    exit(0);
}

