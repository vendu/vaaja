#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mt/mtx.h>
//#include <zero/trix.h>
//#include <v0/mach32.h>
#include <v0/vm/vm.h>
#include <v0/bsp/papilio-pro.h>

#define V0_PC_LK_BIT        (INT32_C(1) << V0_PC_LK_BIT_OFS)
#define V0_PC_LK_BIT_OFS    1
#define V0_READ_BARRIER     1
#define V0_WRITE_BARRIER    2
#define V0_FULL_BARRIER     (V_READ_BARRIER | V0_WRITE_BARRIER)

typedef void (*v0opfunc)(void *, void *, m_word_t);
v0opfunc                   *v0opfunctab[256];
static const unsigned char  v0ham32tab[256]
= {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};
static const unsigned char  vmctz32tab[32]
= {
    31, 0, 27, 1, 28, 13, 23, 2, 29, 21, 19, 14, 24, 16, 3, 7,
    30, 26, 12, 22, 20, 18, 15, 6, 25, 11, 17, 5, 10, 4, 9, 8
};
static const unsigned char  vmclz32tab[129]
= {
  1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  9
};
struct v0vm                 g_vm C_ALIGNED(MACH_CL_SIZE);

#define v0_crash(str)                                                   \
    do {                                                                \
        fprintf(stderr, "%s", str);                                     \
        abort();                                                        \
    } while (0)

#define setopfunc(unit, op, func) opfunctab[(unit << 5) | (op)] = (func)

static void
v0initopfunctab(void)
{
    /* ALU-unit instructions */
    setopfunc(V0_ALU_UNIT, V0_NOT_OP, v0_not);
    setopfunc(V0_ALU_UNIT, V0_NEG_OP, v0_neg);
    setopfunc(V0_ALU_UNIT, V0_AND_OP, v0_and);
    setopfunc(V0_ALU_UNIT, V0_OR_OP, v0_or);
    setopfunc(V0_ALU_UNIT, V0_XOR_OP, v0_xor);
    setopfunc(V0_ALU_UNIT, V0_NOR_OP, v0_nor);
    setopfunc(V0_ALU_UNIT, V0_NOR_OP, v0_nor);
    setopfunc(V0_ALU_UNIT, V0_XNOR_OP, v0_xnor);
    setopfunc(V0_ALU_UNIT, V0_NAND_OP, v0_nand);
    setopfunc(V0_ALU_UNIT, V0_INC_OP, v0_inc);
    setopfunc(V0_ALU_UNIT, V0_DEC_OP, v0_dec);
    setopfunc(V0_ALU_UNIT, V0_SLL_OP, v0_sll);
    setopfunc(V0_ALU_UNIT, V0_SRL_OP, v0_srl);
    setopfunc(V0_ALU_UNIT, V0_SAR_OP, v0_sar);
    setopfunc(V0_ALU_UNIT, V0_ROL_OP, v0_rol);
    setopfunc(V0_ALU_UNIT, V0_ROR_OP, v0_ror);
    setopfunc(V0_ALU_UNIT, V0_RCL_OP, v0_rcl);
    setopfunc(V0_ALU_UNIT, V0_RCR_OP, v0_rcr);
    setopfunc(V0_ALU_UNIT, V0_ADD_OP, v0_add);
    setopfunc(V0_ALU_UNIT, V0_ADDU_OP, v0_addu);
    setopfunc(V0_ALU_UNIT, V0_ADC_OP, v0_adc);
    setopfunc(V0_ALU_UNIT, V0_SUB_OP, v0_sub);
    setopfunc(V0_ALU_UNIT, V0_SBC_OP, v0_sbc);
    setopfunc(V0_ALU_UNIT, V0_MUL_OP, v0_mul);
    setopfunc(V0_ALU_UNIT, V0_UMUL_OP, v0_umul);
    setopfunc(V0_ALU_UNIT, V0_MHI_OP, v0_mhi);
    setopfunc(V0_ALU_UNIT, V0_UMHI_OP, v0_umhi);
    setopfunc(V0_ALU_UNIT, V0_DIV_OP, v0_div);
    setopfunc(V0_ALU_UNIT, V0_UDIV_OP, v0_udiv);
    setopfunc(V0_ALU_UNIT, V0_IDIV_OP, v0_idiv);
    setopfunc(V0_ALU_UNIT, V0_REM_OP, v0_rem);

    /* BIT-unit instructions */
    setopfunc(V0_BIT_UNIT, V0_TST_OP, v0_tst);
    setopfunc(V0_BIT_UNIT, V0_ZEX_OP, v0_zex);
    setopfunc(V0_BIT_UNIT, V0_SEX_OP, v0_sex);
    setopfunc(V0_BIT_UNIT, V0_CLZ_OP, v0_clz);
    setopfunc(V0_BIT_UNIT, V0_CTZ_OP, v0_ctz);
    setopfunc(V0_BIT_UNIT, V0_HAM_OP, v0_ham);
    setopfunc(V0_BIT_UNIT, V0_PAR_OP, v0_par);
    setopfunc(V0_BIT_UNIT, V0_HSH_OP, v0_hsh);
    setopfunc(V0_BIT_UNIT, V0_HUN_OP, v0_hun);
    /* TODO: V0_BEX_OP and V0_BPK_OP currently unimplemented? */

    return;
}

struct v0vm *
v0initvm(struct v0vm *vm, int flg, size_t bramsize, size_t dramsize)
{
    void   *ptr;

    v0initopfunctab();
    mtlkfmtx(&vm->mtx);
    if (flg & V0_VM_READY) {
        mtunlkfmtx(&vm->mtx);

        return vm;
    }
    ptr = calloc(bramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM BRAM...\n");
        abort();
    }
    vm->bram = ptr;
    vm->bramsize = bramsize;
    ptr = calloc(dramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM DRAM...\n");
        abort();
    }
    vm->mem = ptr;
    vm->memsize = dramsize;
    vm->flg |= V0_VM_READY;
    //    v0initbittabs();
    mtunlkfmtx(&vm->mtx);

    return vm;
}

/* ALU-unit 0x01 */

static C_INLINE void
v0_not(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = ~(*(int32_t *)src);
    
    *(int32_t *)dest = ~res;

    return;
}

static C_INLINE void
v0_neg(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = -(*(int32_t *)src);

    *(int32_t *)dest = ~res + 1;

    return;
}

static C_INLINE void
v0_and(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_or(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xor(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res ^= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_nor(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xnor(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res ^= !*(int32_t *)src;
    *(int32_t *)dest = !res;

    return;
}

static C_INLINE void
v0_nand(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = ~*(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

#if 0
static C_INLINE void
v0_nand(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res &= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}
#endif

static C_INLINE void
v0_inc(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val++;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_dec(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val--;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_sll(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    
    uval <<= cnt;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_srl(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;

    uval >>= cnt;
    *(uint32_t *)dest = uval;

    return;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static C_INLINE void
v0_sar(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t dval = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    dval >>= cnt;
    *(int32_t *)dest = dval;

    return;
}

static C_INLINE void
v0_rol(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    int32_t     lo = uval >> (32 - cnt);

    uval <<= cnt;
    uval |= lo;
    *(uint32_t *)dest = uval;
    fprintf(stderr, "%x (%d)\n", uval, cnt);

    return;
}

static C_INLINE void
v0_ror(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    int32_t     lo = uval >> cnt;

    uval <<= 32 - cnt;
    uval |= lo;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_rcl(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    int32_t     cf = g_vm.sysregs[V0_MSW_REGISTER] & V0_MSW_CF_BIT;
    int32_t     cbit = uval & (1U << (31 - cnt));
    int32_t     lo = uval >> (32 - cnt);
    
    uval &= ~cbit;
    uval <<= cnt;
    uval |= lo;
    uval |= !!cf;
    g_vm.sysregs[V0_MSW_REGISTER] |= cbit;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_rcr(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    int32_t     cf = g_vm.sysregs[V0_MSW_REGISTER] & V0_MSW_CF_BIT;
    int32_t     cbit = uval & (1U << (cnt - 1));
    int32_t     lo = uval >> cnt;

    uval &= ~cbit;
    uval <<= 32 - cnt;
    uval |= lo;
    uval |= !!cf;
    g_vm.sysregs[V0_MSW_REGISTER] |= cbit;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_add(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t add = *(int32_t *)src;
    int32_t sum = *(int32_t *)dest;

    sum += add;
    if (sum < add) {
        g_vm.sysregs[V0_MSW_REGISTER] |= V0_MSW_OF_BIT;
    }
    *(int32_t *)dest = sum;

    return;
}

/* ignore overflow */
static C_INLINE void
v0_addu(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t add = *(int32_t *)src;
    int32_t sum = *(int32_t *)dest;

    sum += add;
    *(int32_t *)dest = sum;

    return;
}

static C_INLINE void
v0_adc(volatile void *src, volatile void *dest, m_word_t parm)
{
    v0_crash("ADC not implemented in VM\n");
}

static C_INLINE void
v0_sub(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t sub = *(int32_t *)src;
    int32_t diff = *(int32_t *)dest;

    diff -= sub;
    *(int32_t *)dest = diff;

    return;
}

static C_INLINE void
v0_sbc(volatile void *src, volatile void *dest, m_word_t parm)
{
    v0_crash("SBC not implemented in VM\n");
}

static C_INLINE void
v0_mul(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t mul = *(int32_t *)src;
    int32_t prod = *(int32_t *)dest;

    prod *= mul;
    *(int32_t *)dest = prod;

    return;
}

static C_INLINE void
v0_umul(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    mul = *(uint32_t *)src;
    uint32_t    prod = *(uint32_t *)dest;

    prod *= mul;
    *(uint32_t *)dest = prod;

    return;
}

static C_INLINE void
v0_mhi(volatile void *src, volatile void *dest, m_word_t parm)
{
    int64_t wmul = *(int32_t *)src;
    int64_t wprod = *(int32_t *)dest;
    int32_t prod;

    wprod *= wmul;
    prod = wprod >> 32;
    *(int32_t *)dest = prod;

    return;
}

static C_INLINE void
v0_umhi(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint64_t    wmul = *(uint32_t *)src;
    uint64_t    wprod = *(uint32_t *)dest;
    uint32_t    prod;

    wprod *= wmul;
    prod = wprod >> 32;
    *(uint32_t *)dest = prod;

    return;
}

static C_INLINE void
v0_div(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t num = *(int32_t *)src;
    int32_t den = *(int32_t *)dest;

    den /= num;

    *(int32_t *)dest = den;

    return;
}

static C_INLINE void
v0_udiv(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    num = *(uint32_t *)src;
    uint32_t    den = *(uint32_t *)dest;

    den /= num;

    *(uint32_t *)dest = den;

    return;
}

static C_INLINE void
v0_idiv(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t num = *(int32_t *)src;
    int32_t den = *(int32_t *)dest;

    den = irpdiv(den, num);
    *(int32_t *)dest = den;

    return;
}

static C_INLINE void
v0_rem(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t num = *(int32_t *)src;
    int32_t den = *(int32_t *)dest;

    den %= num;

    *(int32_t *)dest = den;

    return;
}

static C_INLINE void
v0_urem(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    num = *(uint32_t *)src;
    uint32_t    den = *(uint32_t *)dest;

    den %= num;

    *(uint32_t *)dest = den;

    return;
}

/* SUBRT-unit */

static C_INLINE void
v0_cmp(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t arg2 = *(int32_t *)dest;
    int32_t arg1 = *(int32_t *)src;

    v0clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    if (arg1 < arg2) {
        v0setmsw(V0_MSW_LT_BIT);
    } else {
        v0clrmsw(V0_MSW_LT_BIT);
    }
    if (arg1 == arg2) {
        v0setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

static C_INLINE void
v0_ucmp(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t arg2 = *(uint32_t *)dest;
    uint32_t arg1 = *(uint32_t *)src;

    v0clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    if (arg1 < arg2) {
        v0setmsw(V0_MSW_LT_BIT);
    } else {
        v0clrmsw(V0_MSW_LT_BIT);
    }
    if (arg1 == arg2) {
        v0setmsw(V0_MSW_ZF_BIT);
    }
    if (arg1 == arg2) {
        v0setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

/* BIT-unit */

static
vo_tst(volatile void *src, volatile void *dest, m_word_t parm)
{
    ;
}

static C_INLINE void
v0_zex(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    uval;

    switch (parm) {
        case 0:
            uval = *((uint8_t *)src);

            break;
        case 1:
            uval = *((uint16_t *)src);

            break;
        case 2:
            uval = *((uint32_t *)src);

            break;
        case 3:
            fprintf(stderr, "V0 does not support 32+-bit zero-extension...");

            abort();
    }
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_sex(volatile void *src, volatile void *dest, m_word_t parm)
{
    int32_t val;

    switch (parm) {
        case 0:
            val = *((int8_t *)src);

            break;
        case 1:
            val = *((int16_t *)src);

            break;
        case 2:
            val = *((int32_t *)src);

            break;
        case 3:
            fprintf(stderr, "V0 does not support 32+-bit sign-extension...");

            abort();
    }
    *(int32_t *)dest = val;

    return;
}

#if 0
static C_INLINE void
v0_clz(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    uval;

    if (arg1) {
        uval = -(arg1 < 0x1000000);
        uval = -(arg1 < 0x10000);
        uval = -(arg1 < 0x100);
        uval <<= 3;
        uval += 25;
        uval = 33 - uval;
        uval -= vmclz32tab[uval];
    } else {
        uval = 32;
    }
    *(uint32_t *)dest = uval;

    return;
}
#endif

static C_INLINE void
v0_clz(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    uval = !arg1;
    uint32_t    ndx = arg1 & -arg1;
    uint32_t    tmp;

    if (arg1) {
        ndx *= 0x0ef96a62;
        ndx >>= 27;
        tmp = vmctz32tab[ndx];
        uval += tmp;
        uval ^= 31;
    } else {
        uval = 32;
    }
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_ctz(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    uval = !arg1;
    uint32_t    ndx = arg1 & -arg1;
    uint32_t    tmp;

    ndx *= 0x0ef96a62;
    ndx >>= 27;
    tmp = vmctz32tab[ndx];
    uval += tmp;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_ham(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    n = v0ham32tab[arg1 & 0xff];
    uint32_t    n1 = v0ham32tab[(arg1 >> 8) & 0xff];
    uint32_t    n2 = v0ham32tab[(arg1 >> 16) & 0xff];
    uint32_t    n3 = v0ham32tab[(arg1 >> 24) & 0xff];

    n += n1;
    n2 += n3;
    n += n2;
    *(uint32_t *)dest = n;

    return;
}

/* hash-routine */
static C_INLINE void
v0_hsh(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    hash = tmhash32(arg1);

    *(uint32_t *)dest = hash;

    return;
}

/* reverse of the v0_hsh-routine */
static C_INLINE void
v0_hun(volatile void *src, volatile void *dest, m_word_t parm)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    hash = tmunhash32(arg1);

    *(uint32_t *)dest = hash;

    return;
}

static void
v0fetchinst(struct v0vm *vm)
{
    m_word_t        pc;
    m_word_t        newpc;
    m_word_t        ins;

    /* lock PC */
    v0lkbit32(&vm->genregs[V0_PC_REGISTER], V0_PC_LK_BIT_OFS);
    /* adjust PC, fetch instruction */
    pc = vm->genregs[V0_PC_REGISTER];
    pc &= ~V0_PC_LK_BIT;
    ins = ((m_word_t *)vm->mem)[pc];
    pc += sizeof(m_word_t);
    newpc = pc;
    if (ins & V0_INS_IMM32_BIT) {
        newpc += sizeof(m_word_t);
    }
    if (newpc > vm->memsize) {
        fprintf(stderr, "code out of bounds, PC > memsize\n");
        abort();
    }
    /* advance + unlock PC */
    m_atomwrite(&vm->genregs[V0_PC_REGISTER], newpc);
    if (ins & V0_INS_ADR_OFS_BIT) {
        if (ins & V0_INS_IMM32_BIT) {
            /* read immediate 32-bit operand */
            imm = ((m_word_t *)vm->mem)[pc];
            /* adjust PC to point past imm32 */
            newpc += sizeof(m_word_t);
        } else if (ins & V0_INS_VAL8_MASK) {
            /* read immediate 8-bit operand */
            imm = (ins >> V0_INS_VAL8_SHIFT) & 0xff;
        }
    } else if (ins & V0_INS_IMM32_BIT) {
        /* read signed or unsigned immediate 32-bit operand */
        if (!(ins & V0_INS_IMM_UNS_BIT)) {
            imm = ((m_word_t *)vm->mem)[pc];
        } else {
            immu = ((m_uword_t *)vm->mem)[pc];
        }
        newpc += sizeof(m_word_t);
    } else if (ins & V0_INS_VAL8_MASK) {
        imm = (ins >> V0_INS_VAL8_SHIFT) & 0xff;
    }
    switch (ins & V0_INS_MEM_XFER_MASK) {
        case 0:
            src = &((m_word_t *)vm->mem)[reg1];
            dest = &((m_word_t *)vm->mem)[reg2];
        case V0_INS_REG_TO_REG_BIT:
            /* imm -> *%reg2 */
            if (imm) {
                src = &((m_word_t *)vm->mem)[imm];
            } else {
                src = &((m_word_t *)vm->mem)[reg1];
            }
            dest = &((m_word_t *)vm->mem)[reg2];
            
            break;
        case V0_INS_REG_TO_MEM_BIT:
            /*
             * reg1 -> mem[imm]         ADR_OFS
             * reg1 -> mem[reg2]        ADR_BASE
             * reg1 -> mem[imm + reg2]  ADR_OFS | ADR_BASE
             * reg1 -> mem[reg2 + reg3] ADR_NDX | ADR_BASE
             */
            if (ins & V0_INS_ADR_NDX_BIT) {
                adr = reg3;
            } else if (ins & V0_INS_ADR_OFS_BIT) {
                adr = imm;
            }
            if (ins & V0_INS_ADR_BASE_BIT) {
                adr += reg2;
            }
            src = &((m_word_t *)vm->mem)[reg1];
            dest = &((m_word_t *)vm->mem)[adr];
            
            break;
        case V0_INS_MEM_TO_REG_BIT:
            /*
             * mem[imm] -> reg2         ADR_OFS
             * mem[reg1] -> reg2        ADR_BASE
             * mem[imm + reg1] -> reg2  ADR_OFS | ADR_BASE
             * mem[reg1 + reg2] -> reg3 ADR_NDX | ADR_BASE
             */
            if (ins & V0_INS_ADR_NDX_BIT) {
                adr = reg2;
                dest = &((m_word_t *)vm->mem)[reg3];
            } else {
                if (ins & V0_INS_ADR_OFS_BIT) {
                    adr = imm;
                }
                dest = &((m_word_t *)vm->mem)[reg2];
            }
            if (ins & V0_INS_ADR_BASE_BIT) {
                adr += reg1;
            }
            src = &((m_word_t *)vm->mem)[adr];
            
            break;
    }
    
    return;
}

int
main(int argc, char *argv[])
{
    int         i;
    int         j;
    uint32_t    uval;

    v0initvm(&g_vm, 0, PAPILIO_PRO_BRAM_SIZE, PAPILIO_PRO_DRAM_SIZE);
    g_vm.genregs[V0_R1_REGISTER] = 0x55555555;
    g_vm.genregs[V0_R2_REGISTER] = 1;
    v0_rol(&g_vm.genregs[V0_R2_REGISTER], &g_vm.genregs[V0_R1_REGISTER], 0);
    fprintf(stderr, "ROL: %x\n", g_vm.genregs[V0_R1_REGISTER]);
    g_vm.genregs[V0_R1_REGISTER] = 0xaaaaaaaa;
    g_vm.genregs[V0_R2_REGISTER] = 1;
    v0_ror(&g_vm.genregs[V0_R2_REGISTER], &g_vm.genregs[V0_R1_REGISTER], 0);
    fprintf(stderr, "ROR: %x\n", g_vm.genregs[V0_R1_REGISTER]);
    g_vm.genregs[V0_R1_REGISTER] = 0x55555555;
    g_vm.genregs[V0_R2_REGISTER] = 3;
    v0_rcl(&g_vm.genregs[V0_R2_REGISTER], &g_vm.genregs[V0_R1_REGISTER], 0);
    fprintf(stderr, "RCL: %x (CF == %x)\n", g_vm.genregs[V0_R1_REGISTER],
            g_vm.sysregs[V0_MSW_REGISTER]);
    g_vm.genregs[V0_R1_REGISTER] = 0xaaaaaaaa;
    g_vm.genregs[V0_R2_REGISTER] = 3;
    v0_rcr(&g_vm.genregs[V0_R2_REGISTER], &g_vm.genregs[V0_R1_REGISTER], 0);
    fprintf(stderr, "RCR: %x (CF == %x)\n", g_vm.genregs[V0_R1_REGISTER],
            g_vm.sysregs[V0_MSW_REGISTER]);
    for (uval = UINT32_C(1) << 31 ; (uval) ; uval >>= 1) {
        g_vm.genregs[V0_R1_REGISTER] = uval;
        v0_clz(&g_vm.genregs[V0_R1_REGISTER], &g_vm.genregs[V0_R2_REGISTER], 0);
        fprintf(stderr, "CLZ: %d\n", g_vm.genregs[V0_R2_REGISTER]);
    }
    g_vm.genregs[V0_R1_REGISTER] = 0;
    v0_clz(&g_vm.genregs[V0_R1_REGISTER], &g_vm.genregs[V0_R2_REGISTER], 0);
    fprintf(stderr, "CLZ: %d\n", g_vm.genregs[V0_R2_REGISTER]);
    for (uval = UINT32_C(1) << 31 ; (uval) ; uval >>= 1) {
        g_vm.genregs[V0_R1_REGISTER] = uval;
        v0_ctz(&g_vm.genregs[V0_R1_REGISTER], &g_vm.genregs[V0_R2_REGISTER], 0);
        fprintf(stderr, "CTZ: %d\n", g_vm.genregs[V0_R2_REGISTER]);
    }
    g_vm.genregs[V0_R1_REGISTER] = 0;
    v0_ctz(&g_vm.genregs[V0_R1_REGISTER], &g_vm.genregs[V0_R2_REGISTER], 0);
    fprintf(stderr, "CTZ: %d\n", g_vm.genregs[V0_R2_REGISTER]);
    fflush(stderr);
    
    exit(0);
}

