#include <limits.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/atomic.h>
#include <v0/regs.h>
#include <v0/types.h>

#define V0_SIGN_BIT         (INT32_C(1) << 31)

#define v0_setmsw(bits)     (g_vm.sysregs[V0_MSW_REGISTER] |= (bits))
#define v0_clrmsw(bits)     (g_vm.sysregs[V0_MSW_REGISTER] &= ~(bits))

#define v0getfp(vm)         ((vm)->genregs[V0_FP_REGISTER])
#define v0getsp(vm)         ((vm)->genregs[V0_SP_REGISTER])
#define v0getlr(vm)         ((vm)->genregs[V0_LR_REGISTER])
#define v0getpc(vm)         ((vm)->genregs[V0_PC_REGISTER])

#define V0_VM_READY         (1 << 31)
struct v0vm {
    volatile m_atomic_t     mtx;
    long                    flg;
    volatile m_adr_t       *mem;
    size_t                  memsize;
    volatile m_word_t       genregs[V0_GENERAL_REGISTERS];
    volatile m_word_t       sysregs[V0_GENERAL_REGISTERS];
    volatile m_word_t       segregs[V0_SEGMENT_REGISTERS];
};

