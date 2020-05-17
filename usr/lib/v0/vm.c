#include <stdint.h>
#include <stdio.h>
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <v0/mach32.h>
#include <v0/types.h>
#include <v0/vm.h>
#include <v0/vm.h>

struct vm g_vm;

static C_INLINE void
v0_not(void *src, void *dest)
{
    int32_t res = ~((int32_t *)src);
    
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_neg(void *src, void *dest)
{
    int32_t res = -(*(int32_t *)src);

    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_and(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res &= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_or(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res |= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xor(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res ^= *(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_nor(void *src, void *dest)
{
    int32_t res = *(int32_t *)dest;

    res |= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_xnor(void *src, void *dest)
{
    int32_t res = !*(int32_t *)dest;

    res ^= !*(int32_t *)src;
    *(int32_t *)dest = !res;

    return;
}

static C_INLINE void
v0_nand(void *src, void *dest)
{
    int32_t res = !*(int32_t *)dest;

    res &= !*(int32_t *)src;
    *(int32_t *)dest = res;

    return;
}

static C_INLINE void
v0_inc(void *src, void *dest)
{
    int32_t val = *(int32_t *)src;

    val++;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_dec(void *src, void *dest)
{
    int32_t val = *(int32_t *)src;

    val--;
    *(int32_t *)dest = val;

    return;
}

static C_INLINE void
v0_sll(void *src, void *dest)
{
    uint32_t uval = *(uint32_t *)dest;
    int32_t  cnt = *(int32_t *)src;
    
    uval <<= cnt;
    *(uint32_t *)dest = uval;

    return;
}

static C_INLINE void
v0_srl(void *src, void *dest)
{
    uint32_t uval = *(uint32_t *)dest;
    int32_t  cnt = *(int32_t *)src;

    uval >>= cnt;
    *(uint32_t *)dest = uval;

    return;
}

/* NOTE: we assume C's right-shift to be arithmetic for signed types */
static C_INLINE void
v0_sar(void *src, void *dest)
{
    int32_t sval = *(int32_t *)dest;
    int32_t cnt = *(int32_t *)src;

    sval >>= cnt;
    *(int32_t *)dest = (int32_t)sval;

    return;
}

static C_INLINE void
v0_cmp(void *src, void *dest)
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
        vm_setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

static C_INLINE void
v0_ucmp(void *src, void *dest)
{
    uint32_t arg2 = *(uint32_t *)dest;
    uint32_t arg1 = *(uint32_t *)src;

    vm_clrmsw(V0_MSW_ZF_BIT | V0_MSW_LT_BIT);
    if (arg1 < arg2) {
        v0_setmsw(V0_MSW_LT_BIT);
    } else {
        v0_clrmsw(V0_MSW_LT_BIT);
    }
    if (arg1 == arg2) {
        vm_setmsw(V0_MSW_ZF_BIT);
    }
    if (arg1 == arg2) {
        vm_setmsw(V0_MSW_ZF_BIT);
    }

    return;
}

struct vm *
v0initvm(int flg, size_t dramsize)
{
    void   *ptr;
    
    mtlkfmtx(&g_vm.mtx);
    if (flg & V0_VM_READY) {
        mtunlkfmtx(&g_vm.mtx);

        return;
    }
    ptr = calloc(dramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM memory...\n");
        abort();
    }
    g_vm.mem = ptr;
    g_vm.memsize = dramsize;
    g_vm.flg |= V0_VM_READY;
    mtunlkfmtx(&g_vm.mtx);
}

v0addvmmem(int type, size_t size, v0adr adr);
