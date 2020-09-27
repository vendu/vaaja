#include <stdint.h>
#include <zero/cdefs.h>

/* LOGIC-unit #0 */

static void
v0_not32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;

    res = ~res;
    *(int32_t *)dest = res;
}

static void
v0_orr32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res |= val;

    *(int32_t *)dest = res;
}

static void
v0_xor32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res ^= val;

    *(int32_t *)dest = res;
}

static void
v0_and32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res &= val;

    *(int32_t *)dest = res;
}

static void
v0_nor32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res |= val;

    *(int32_t *)dest = ~res;
}

static void
v0_xnor32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res ^= val;

    *(int32_t *)dest = ~res;
}

static void
v0_nand32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    res &= val;

    *(int32_t *)dest = res;
}

/* SHIFT-unit #1 */

static void
v0_slr32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;

    ures >>= cnt;

    *(int32_t *)dest = ures;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static void
v0_sar32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;

    res >>= cnt;

    *(uint32_t *)dest = res;
}

static void
v0_sll32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;

    res <<= cnt;

    *(int32_t *)dest = res;
}

static void
v0_ror32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;
    int32_t     lo = ures >> cnt;

    //    ures <<= 32 - cnt;
    ures <<= -cnt & 31;
    ures |= lo;
    *(uint32_t *)dest = ures;

    return;
}

static void
v0_rol32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;
    int32_t     lo = ures >> (-cnt & 31);
    //    int32_t     lo = ures >> (32 - cnt);

    ures <<= cnt;
    ures |= lo;
    *(uint32_t *)dest = ures;

    return;
}

static void
v0_rcr32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;
    int32_t     cf = g_vm.sysregs[V0_MSW_REGISTER] & V0_MSW_CF_BIT;
    int32_t     cbit = ures & (1U << (cnt - 1));
    int32_t     lo = ures >> cnt;

    ures &= ~cbit;
    //    ures <<= 32 - cnt;
    ures <<= -cnt & 31 ;
    ures |= lo;
    ures |= !!cf;
    g_vm.sysregs[V0_MSW_REGISTER] |= cbit;
    *(uint32_t *)dest = ures;

    return;
}

static void
v0_rcl32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;
    int32_t     cnt = *(int32_t *)src2;
    int32_t     cf = g_vm.sysregs[V0_MSW_REGISTER] & V0_MSW_CF_BIT;
    int32_t     cbit = ures & (1U << (31 - cnt));
    //    int32_t     lo = ures >> (32 - cnt);
    int32_t     lo = ures >> (-cnt & 31);

    ures &= ~cbit;
    ures <<= cnt;
    ures |= lo;
    ures |= !!cf;
    g_vm.sysregs[V0_MSW_REGISTER] |= cbit;
    *(uint32_t *)dest = ures;

    return;
}

/* ALU-unit 0x01 */

static void
v0_inc32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;

    res++;
    *(int32_t *)dest = res;

    return;
}

static void
v0_uinc32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    res = *(uint32_t *)src1;

    res++;
    *(uint32_t *)dest = res;

    return;
}

static void
v0_dec32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     res = *(int32_t *)src1;

    res--;
    *(int32_t *)dest = res;

    return;
}

static void
v0_udec32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    res = *(uint32_t *)src1;

    res--;
    *(uint32_t *)dest = res;

    return;
}

static void
v0_add32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     sum = *(int32_t *)src1;
    int32_t     val = *(int32_t *)src2;

    sum += val;
    if (sum < val) {
        g_vm.sysregs[V0_MSW_REGISTER] |= V0_MSW_OF_BIT;
    }
    *(int32_t *)dest = sum;

    return;
}

/* ignore overflow */
static void
v0_uadd32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    sum = *(uint32_t *)src1;
    uint32_t    val = *(uint32_t *)src2;

    sum += val;
    *(uint32_t *)dest = sum;

    return;
}

static void
v0_adc32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    v0_crash("ADC not implemented in VM\n");
}

static void
v0_sub32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     diff = *(int32_t *)dest;
    int32_t     val = *(int32_t *)src2;

    diff -= val;
    *(int32_t *)dest = diff;

    return;
}

static void
v0_usub32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    diff = *(uint32_t *)dest;
    uint32_t    val = *(uint32_t *)src2;

    diff -= val;
    *(uint32_t *)dest = diff;

    return;
}

static void
v0_sbc32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    v0_crash("SBC not implemented in VM\n");
}

/* MULDIV-unit 0x03 */

static void
v0_mul32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t prod = *(int32_t *)dest;
    int32_t val = *(int32_t *)src2;

    prod *= val;
    *(int32_t *)dest = prod;

    return;
}

static void
v0_umul32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    prod = *(uint32_t *)dest;
    uint32_t    val = *(uint32_t *)src2;

    prod *= val;
    *(uint32_t *)dest = prod;

    return;
}

static void
v0_mhi32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int64_t wprod = *(int32_t *)dest;
    int64_t wval = *(int32_t *)src21;
    int32_t prod;

    wprod *= wval;
    prod = wprod >> 32;
    *(int32_t *)dest = prod;

    return;
}

static void
v0_umhi32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint64_t    wprod = *(uint32_t *)dest;
    uint64_t    wval = *(uint32_t *)src2;
    uint32_t    prod;

    wprod *= wval;
    prod = wprod >> 32;
    *(uint32_t *)dest = prod;

    return;
}

static void
v0_div32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t den = *(int32_t *)dest;
    int32_t num = *(int32_t *)src2;

    den /= num;

    *(int32_t *)dest = den;

    return;
}

static void
v0_udiv32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    den = *(uint32_t *)dest;
    uint32_t    num = *(uint32_t *)src2;

    den /= num;

    *(uint32_t *)dest = den;

    return;
}

static void
v0_idiv32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t den = *(int32_t *)dest;
    int32_t num = *(int32_t *)src2;

    den = irpdiv(den, num);
    *(int32_t *)dest = den;

    return;
}

static void
v0_rem32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t den = *(int32_t *)dest;
    int32_t num = *(int32_t *)src2;

    den %= num;

    *(int32_t *)dest = den;

    return;
}

static void
v0_urem32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    den = *(uint32_t *)dest;
    uint32_t    num = *(uint32_t *)src2;

    den %= num;

    *(uint32_t *)dest = den;

    return;
}

static void
v0_irp32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    v0crash("IRP not implemented in VM\n");
}

static void
v0_uirp32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    v0crash("UIRP not implemented in VM\n");
}

/* BIT-unit 0x04 */

static void
v0_zexb32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint8_t *)src1;

    *(uint32_t *)dest = ures;

    return;
}

static void
v0_zexh32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint16_t *)src1;

    *(uint32_t *)dest = ures;

    return;
}

static void
v0_zex32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;

    *(uint32_t *)dest = ures;

    return;
}

static void
v0_sexb32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint8_t *)src1;

    ures |= 0xffffff00;
    *(uint32_t *)dest = res;

    return;
}

static void
v0_sexh32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint16_t *)src1;

    ures |= 0xffff0900;
    *(uint32_t *)dest = res;

    return;
}

static void
v0_sex32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures = *(uint32_t *)src1;

    *(uint32_t *)dest = res;

    return;
}

static void
v0_neg32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t ures = *(uint32_t *)src1;

    res = ~res;
    res++;

    *(int32_t *)dest = res;

    return;
}

static void
v0_ham32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
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

static void
v0_ctz32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
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

static void
v0_clz32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
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

/* hash-routine */
static void
v0_hsh32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    hash = tmhash32(arg1);

    *(uint32_t *)dest = hash;

    return;
}

/* reverse of the v0_hsh-routine */
static void
v0_hun32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    arg1 = *(uint32_t *)src;
    uint32_t    hash = tmunhash32(arg1);

    *(uint32_t *)dest = hash;

    return;
}

/* COMP-unit 0x05 */

static void
v0_cmp32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
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

static void
v0_cmu32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
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

static void
v0_tst32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    arg1 = *(uint32_t *)src1;
    uint32_t    arg2 = *(uint32_t *)src2;

    *(uint32_t *)dest = arg1 & arg2;
}

static void
v0_teq32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    arg1 = *(uint32_t *)src1;
    uint32_t    arg2 = *(uint32_t *)src2;

    *(uint32_t *)dest = arg1 ^ arg2;
}

/* BRANCH-unit 0x06 */

static void
v0_beq32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condeq(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bne32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condne(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

v0_bgt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condgt(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_blt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condlt(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bge32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condge(msw)) {
        && !!(msw & V0_MSW_OF_BIT)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_ble32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condle(msw)) {
        && (!!(msw & V0_MSW_SF_BIT) != !! (msw & V0_MSW_OF_BIT))) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bhs32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condhs(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_blo32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condlo(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bmi32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condmi(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bpl32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condpl(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bof32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condof(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bno32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condno(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bhi32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condhi(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_bls32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    msw = g_vm.sysregs[V0_MSW_REGISTER];
    int32_t     adr = *(int32_t *)dest;
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 0;
    int32_t     ofs = (src1) ? *(int32_t *)src2 : 0;

    if (v0condls(msw)) {
        adr += ndx;
        adr += ofs;
        g_vm.genregs[V0_PC_REGISTER] = adr;
    }

    return;
}

static void
v0_skp32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     adr = g_vm.genregs[V0_PC_REGISTER];
    int32_t     ndx = (src1) ? *(int32_t *)src1 : 4;

    adr += ndx;
    g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}

static void
v0_jmp32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     adr = *(int32_t *)dest;

    g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}

static void
v0_jmi32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    adr = *(uint32_t *)dest;

    *(uint32_t *)&g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}

static void
v0_jmr32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     adr = *(int32_t *)dest;

    g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}

static void
v0_jal32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     adr = *(int32_t *)dest;
    int32_t     pc = g_vm.genregs[V0_PC_REGISTER];

    g_vm.genregs[V0_LR_REGISTER] = pc;

    return;
}

static void
v0_jpc32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     adr = *(int32_t *)dest;
    int32_t     pc = g_vm.genregs[V0_PC_REGISTER];

    adr += pc;
    g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}

#if 0
static void
v0_jmf32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    int32_t     seg = *(int32_t *)dest;
    int32_t     ofs = *(int32_t *)src1;
    int32_t     pc = g_vm.genregs[V0_PC_REGISTER];

    adr += pc;
    g_vm.genregs[V0_PC_REGISTER] = adr;

    return;
}
#endif

static void
v0_srt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    return;
}

static void
v0_srt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    return;
}

static void
v0_ret32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    return;
}

static void
v0_sme32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    return;
}

static void
v0_smx32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    return;
}

/* ATOMIC-unit 0x08 */

/*
 * - *dest++; src1 = dest;
 */
static void
v0_xinc32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t     val;

    v0lkbus();
    val = *(int32_t *)dest;
    val++;
    *(int32_t *)dest = val;
    *(int32_t *)src1 = val;
    v0_crash("XINC not implemented in VM\n");
    v0unlkbus();
}

/*
 * - *dest--; src1 = dest;
 */
static void
v0_xdec32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t     val;

    v0lkbus();
    val = *(int32_t *)dest;
    val--;
    *(int32_t *)dest = val;
    *(int32_t *)src1 = val;
    v0_crash("XINC not implemented in VM\n");
    v0unlkbus();
}

/* *dest += src1; src2 = *dest; */
static void
v0_xadd32(const void * C_RESTRICT dest,
          const void * C_RESTRICT src1,
          const void * C_RESTRICT src2)
{
    int32_t     val;
    int32_t     sum = *(int32_t *)src1;

    v0lkbus();
    val = *(int32_t *)dest;
    sum += val;
    *(int32_t *)src2 = val;
    v0unlkbus();
}

static void
v0_bst32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    upos = *(uint32_t *)src1;
    uint32_t    uval = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    *(uint32_t *)dest |= uval;
    v0unlkbus();

    return;
}

static void
v0_bcl32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    upos = *(uint32_t *)src1;
    uint32_t    uval = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    *(uint32_t *)dest &= ~uval;
    v0unlkbus();

    return;
}

static void
v0_bfl32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    upos = *(uint32_t *)src1;
    uint32_t    uval = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    *(uint32_t *)dest ^= uval;
    v0unlkbus();

    return;
}

static void
v0_bic32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t uval = *(uint32_t *)src1;
    uint32_t ures = *(uint32_t *)src2;

    ures &= ~uval;
    v0lkbus();
    *(uint32_t *)dest = ures;
    v0unlkbus;

    return;
}

static void
v0_bts32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures;
    uint32_t    uval = *(uint32_t *)src1;
    uint32_t    upos = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    ures = *(uint32_t *)dest;
    if (ures & uval) {
        *(uint32_t *)src2 = UINT32_C(1);
    } else {
        ures |= uval;
        *(uint32_t *)dest = ures;
    }
    v0unlkbus();

    return;
}

static void
v0_btc32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures;
    uint32_t    uval = *(uint32_t *)src1;
    uint32_t    upos = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    ures = *(uint32_t *)dest;
    if (!(ures & uval)) {
        *(uint32_t *)src2 = UINT32_C(0);
    } else {
        ures ^= uval;
        *(uint32_t *)dest = ures;
    }
    v0unlkbus();

    return;
}

static void
v0_bfl32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    ures;
    uint32_t    uval = *(uint32_t *)src1;
    uint32_t    upos = UINT32_C(1);

    uval <<= upos;
    v0lkbus();
    ures = *(uint32_t *)dest;
    if (ures & uval) {
        *(uint32_t *)src2 = UINT32_C(1);
    } else {
        *(uint32_t *)src2 = UINT32_C(0);
    }
    ures ^= uval;
    *(uint32_t *)dest = ures;
    v0unlkbus();

    return;
}

/*
 * - if *dest == src1 { *dest = src2; src2 = 0 } else { src2 = 1 }
 */
static void
v0_cas32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    uint32_t    udest;
    uint32_t    uwant = *(uint32_t *)src1;
    uint32_t    uval = *(uint32_t *)src2;

    v0lkbus();
    udest = *(uint32_t)dest;
    if (udest == uwant) {
        *(uint32_t *)dest = uval;
        uval = UINT32_C(0);
    } else {
        uval = UINT32_C(1);
    }
    *(uint32_t *)src2 = uval;
    v0unlkbus();
}

static void
v0_smt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    v0lkpage(dest);
}

static void
v0_fmt32(const void * C_RESTRICT dest,
         const void * C_RESTRICT src1,
         const void * C_RESTRICT src2)
{
    v0unlkpage(dest);
}

