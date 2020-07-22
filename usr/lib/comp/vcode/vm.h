#ifndef __SYS_VCODE_VM_H__
#define __SYS_VCODE_VM_H__

#include <stdint.h>
#include <mach/param.h>

#define V_INTR_VECTOR_SIZE      V_PAGE_SIZE

/* VMINE CONTROL BITS */
#if (WORDSIZE == 4)
#define V_FLG_MAX_SHIFT         31
#elif (WORDSIZE == 8)
#define V_FLG_MAX_SHIFT         63
#endif
#define V_INTR_BIT              (1 << 0)
#define V_ZERO_BIT              (1 << 1)
#define V_OVERFLOW_BIT          (1 << 2)
#define V_CARRY_BIT             (1 << 3)
#define V_PARITY_BIT            (1 << 4)
#define V_ALIGN_BIT             (1 << 5)
#define V_ERROR_FLAG            (1 << 31)
#define V_ERROR_MASK            0xff

struct vcodevm {
    struct vcodeinst   *pc;
    void               *proc();
    void               *asm();
    intmax_t            flg;
};

#endif /* __SYS_VCODE_VM_H__ */

