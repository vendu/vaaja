#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mt/mtx.h>
//#include <zero/trix.h>
//#include <v0/mach32.h>
#include <v0/vm/vm.h>
#include <v0/vm/bsp/papilio-pro.h>

#define V0_PC_LK_BIT        (INT32_C(1) << V0_PC_LK_BIT_OFS)
#define V0_PC_LK_BIT_OFS    1
#define V0_READ_BARRIER     1
#define V0_WRITE_BARRIER    2
#define V0_FULL_BARRIER     (V_READ_BARRIER | V0_WRITE_BARRIER)

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
};struct v0vm                 g_vm C_ALIGNED(MACH_CL_SIZE);

struct v0vm *
v0initvm(struct v0vm *vm, int flg, size_t bramsize, size_t dramsize)
{
    void   *ptr;

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

static void
v0_not(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = ~(*(int32_t *)src);
    
    *(int32_t *)dest = ~res;

    return;
}

static void
v0_neg(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = -(*(int32_t *)src);

    *(int32_t *)dest = ~res + 1;

    return;
}

static void
v0_and(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static void
v0_or(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static void
v0_xor(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res ^= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static void
v0_nor(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = *(int32_t *)dest;

    res |= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static void
v0_xnor(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res ^= !*(int32_t *)src;
    *(int32_t *)dest = !res;

    return;
}

static void
v0_nand(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = ~*(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

#if 0
static void
v0_nand(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t res = !*(int32_t *)dest;

    res &= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}
#endif

static void
v0_inc(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val++;
    *(int32_t *)dest = val;

    return;
}

static void
v0_dec(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t val = *(int32_t *)src;

    val--;
    *(int32_t *)dest = val;

    return;
}

static void
v0_sll(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    
    uval <<= cnt;
    *(uint32_t *)dest = uval;

    return;
}

static void
v0_srl(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;

    uval >>= cnt;
    *(uint32_t *)dest = uval;

    return;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static void
v0_sar(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t dval = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    dval >>= cnt;
    *(int32_t *)dest = dval;

    return;
}

static void
v0_rol(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_ror(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    uval = *(uint32_t *)dest;
    int32_t     cnt = *(int32_t *)src;
    int32_t     lo = uval >> cnt;

    uval <<= 32 - cnt;
    uval |= lo;
    *(uint32_t *)dest = uval;

    return;
}

static void
v0_rcl(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_rcr(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_add(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 += s32;
    *(int32_t *)dest = d32;

    return;
}

static void
v0_addu(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    ;
}

static void
v0_adc(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    ;
}

static void
v0_sub(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 -= s32;
    *(int32_t *)dest = d32;

    return;
}

static void
v0_sbc(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    ;
}

static void
v0_mul(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 *= s32;
    *(int32_t *)dest = d32;

    return;
}

static void
v0_umul(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    s32 = *(uint32_t *)src;
    uint32_t    d32 = *(uint32_t *)dest;

    d32 *= s32;
    *(uint32_t *)dest = d32;

    return;
}

static void
v0_mhi(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int64_t s64 = *(int32_t *)src;
    int64_t d64 = *(int32_t *)dest;
    int32_t d32;

    d64 *= s64;
    d32 = d64 >> 32;
    *(int32_t *)dest = d32;

    return;
}

static void
v0_umhi(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint64_t    s64 = *(uint32_t *)src;
    uint64_t    d64 = *(uint32_t *)dest;
    uint32_t    d32;

    d64 *= s64;
    d32 = d64 >> 32;
    *(uint32_t *)dest = d32;

    return;
}

static void
v0_idiv(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 = irpdiv(d32, s32);
    *(int32_t *)dest = d32;

    return;
}

static void
v0_div(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 /= s32;

    *(int32_t *)dest = d32;

    return;
}

static void
v0_udiv(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    s32 = *(uint32_t *)src;
    uint32_t    d32 = *(uint32_t *)dest;

    d32 /= s32;

    *(uint32_t *)dest = d32;

    return;
}

static void
v0_rem(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    int32_t s32 = *(int32_t *)src;
    int32_t d32 = *(int32_t *)dest;

    d32 %= s32;

    *(int32_t *)dest = d32;

    return;
}

static void
v0_urem(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    uint32_t    s32 = *(uint32_t *)src;
    uint32_t    d32 = *(uint32_t *)dest;

    d32 %= s32;

    *(uint32_t *)dest = d32;

    return;
}

/* BIT-unit */

static
vo_tst(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
{
    ;
}

static void
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

static void
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
static void
v0_clz(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_clz(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_ctz(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

/* SUBRT-unit */

static void
v0_cmp(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static void
v0_ucmp(volatile void *src, volatile void *dest, C_UNUSED m_word_t parm)
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

static int
v0fetchinst(struct v0vm *vm)
{
    volatile void  *src;
    volatile void  *dest;
    m_word_t        imm = 0;
    m_word_t        parm;
    m_reg_t         reg1;
    m_reg_t         reg2;
    m_reg_t         reg3;
    m_adr_t         adr;
    m_word_t        pc;
    m_word_t        newpc;
    m_word_t        ins;

    /* lock PC */
    v0lkbit32(&vm->genregs[V0_PC_REGISTER], V0_PC_LK_BIT_OFS);
    /* adjust PC, fetch instruction */
    pc = vm->genregs[V0_PC_REGISTER];
    /* read source, destination, and extra registers */
    reg1 = (ins >> V0_INS_SRC_SHIFT) & 0x0f;
    reg2 = (ins >> V0_INS_DEST_SHIFT) & 0x0f;
    pc &= ~V0_PC_LK_BIT;
    reg3 = (ins >> V0_INS_REG3_SHIFT) & 0x0f;
    parm = ins >> V0_INS_PARM_SHIFT;
    newpc = pc + sizeof(m_word_t);
    ins = ((m_word_t *)vm->mem)[pc];
    if (ins & V0_INS_ADR_OFS_BIT) {
        if (ins & V0_INS_IMM32_BIT) {
            /* read immediate 32-bit operand */
            imm = ((m_word_t *)vm->mem)[newpc];
            /* adjust PC to point past imm32 */
            newpc += sizeof(m_word_t);
        } else if (ins & V0_INS_VAL8_MASK) {
            /* read immediate 8-bit operand */
            imm = (ins >> V0_INS_VAL8_SHIFT) & 0xff;
        }
    }
    switch (ins & V0_INS_MEM_XFER_MASK) {
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
            /* reg1 -> mem[imm], reg1 -> mem[imm + reg2] */
            src = &((m_word_t *)vm->mem)[reg1];
            if (ins & V0_INS_ADR_OFS_BIT) {
                adr = imm;
            }
            if (ins & V0_INS_ADR_BASE_BIT) {
                adr += reg2;
            }
            dest = &((m_word_t *)vm->mem)[adr];
            
            break;
        case V0_INS_MEM_TO_REG_BIT:
            /* imm -> reg2, mem[reg1] -> reg2, mem[reg1 + reg2] -> reg3 */
            if (ins & V0_INS_ADR_OFS_BIT) {
                adr = imm;
            }
            if (ins & V0_INS_ADR_BASE_BIT) {
                adr += reg1;
            }
            src = &((m_word_t *)vm->mem)[adr];
            
            break;
    }
    m_atomwrite(&vm->genregs[V0_PC_REGISTER], newpc);

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

