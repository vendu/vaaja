#ifndef __V0_VM_VM_H__
#define __V0_VM_VM_H__

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <mach/atomic.h>
#include <fxp/fxp.h>
#include <sys/voima/vm/unit.h>
#include <sys/voima/vm/regs.h>

struct v0vm;
extern struct v0vm          g_vm;

#define V0_SIGN_BIT         (INT32_C(1) << 31)

#define v0lkbus()           mtlkfmtx(&g_vm.buslk)
#define v0unlkbus()         mtunlkfmtx(&g_vm.buslk)

#define v0thrid()           (g_vm.thr)

#define v0setmsw(bits)      (g_vm.sysregs[V0_MSW_REGISTER] |= (bits))
#define v0clrmsw(bits)      (g_vm.sysregs[V0_MSW_REGISTER] &= ~(bits))

#define v0getfp(vm)         ((vm)->genregs[V0_FP_REGISTER])
#define v0getsp(vm)         ((vm)->genregs[V0_SP_REGISTER])
#define v0getlr(vm)         ((vm)->genregs[V0_LR_REGISTER])
#define v0getpc(vm)         ((vm)->genregs[V0_PC_REGISTER])

#define v0condeq(msw)       ((msw) & V0_MSW_ZF_BIT)
#define v0condne(msw)       (!((msw) & V0_MSW_ZF_BIT))
#define v0condgt(msw)       (!((msw) & V0_MSW_ZF_BIT)                   \
                             && !!((msw) & V0_MSW_SF_BIT)               \
                             == !!((msw) & V0_MSW_OF_BIT))
#define v0condlt(msw)       (!!((msw) & V0_MSW_SF_BIT)                  \
                             != !!((msw) & V0_MSW_OF_BIT))
#define v0condge(msw)       (!!((msw) & V0_MSW_SF_BIT)                  \
                             == !!((msw) & V0_MSW_OF_BIT))
#define v0condle(msw)       (((msw) & V0_MSW_ZF_BIT)                    \
                             || !!((msw) & V0_MSW_SF_BIT)               \
                             != !!((msw) & V0_MSW_OF_BIT))
#define v0condhs(msw)       ((msw) & V0_MSW_CF_BIT)
#define v0condlo(msw)       (!((msw) & V0_MSW_CF_BIT))
#define v0condmi(msw)       ((msw) & V0_MSW_SF_BIT)
#define v0condpl(msw)       (!((msw) & V0_MSW_SF_BIT))
#define v0condof(msw)       ((msw) & V0_MSW_OF_BIT)
#define v0condno(msw)       (!((msw) & V0_MSW_OF_BIT))
#define v0condhi(msw)       (!((msw) & V0_MSW_CF_BIT)                   \
                             && (!(msw) & V0_MSW_ZF_BIT))
#define v0condls(msw)       (!((msw) & V0_MSW_ZF_BIT)                   \
                             && !((msw) & V0_MSW_CF_BIT))

/* status-flags */
#define V0_VM_READY         (1 << 31)
#define V0_VM_TERMINATE     (1 << 30)
#define V0_VM_RESET         (1 << 29)
#define v0vmexitstat(vm)    ((vm)->status & 0xff)
struct v0vm {
    volatile m_atomic_t     mtx;
    volatile m_atomic_t     buslk;
    volatile long           status;
    volatile void          *bram;
    size_t                  bramsize;
    volatile void          *mem;
    size_t                  memsize;
    struct v0thr           *thr;
};

#endif /* __V0_VM_VM_H__ */

