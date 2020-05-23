#include <limits.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/atomic.h>
#include <v0/regs.h>
#include <v0/types.h>

#define V0_SIGN_BIT         (INT32_C(1) << 31)

#define v0_setmsw(bits)     (g_vm.sysregs[V0_MSW_REGISTER] |= (bits))
#define v0_clrmsw(bits)     (g_vm.sysregs[V0_MSW_REGISTER] &= ~(bits))

#define V0_VM_READY         (1 << 31)
struct vm {
    volatile m_atomic_t     mtx;
    long                    flg;
    m_adr_t                *mem;
    size_t                  memsize;
    m_word_t                genregs[V0_GENERAL_REGISTERS];
    m_word_t                sysregs[V0_GENERAL_REGISTERS];
    m_word_t                segregs[V0_SEGMENT_REGISTERS];
};
