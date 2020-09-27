#ifndef __MACH_V0_TYPES_H__
#define __MACH_V0_TYPES_H__

#include <stdint.h>
#include <mach/v0/regs.h>

/* machine types */
typedef int64_t                 m_reg_t;
typedef uint64_t                m_ureg_t;
#if defined(V0_HAVE_LONG64)
typedef int64_t                 m_long_t;
typedef uint64_t                m_ulong_t;
typedef uint64_t                m_size_t;
#else
typedef uint32_t                m_size_t;
#endif
typedef int32_t                 m_word_t;
typedef uint32_t                m_uword_t;
typedef int16_t                 m_half_t;
typedef uint16_t                m_uhalf_t;
typedef int8_t                  m_byte_t;
typedef uint8_t                 m_ubyte_t;
typedef uint32_t                m_adr_t;
typedef void                   *m_ptr_t;
typedef int64_t                 m_long_t;
typedef uint64_t                m_ulong_t;

/* coprocessor types */
typedef float                   m_fpsingle_t;
typedef double                  m_fpdouble_t;
typedef int64_t                 m_fxp_t;

struct v0romparm {
    void                       *buf;
    m_size_t                    bufsize;
};

struct v0trapframe {
    int32_t                     code;
    int32_t                     msw;
    int32_t                     sp1;
    int32_t                     pc1;
};

struct v0retframe {
    int32_t                     oldfp;
    int32_t                     retadr;
    int32_t                     r5_r13[V0_CALLEE_SAVE_REGISTERS];
};

struct v0callframe {
    int32_t                     r1_4[V0_CALLER_SAVE_REGISTERS];
    int32_t                     r14_r15[4];     // LR, PC
    int32_t                     xr0_xr15[16];
};

struct v0seg {
    uint32_t                info;   // page-address + flags
    uint32_t                lim;
};

struct v0thr {
    int32_t                 id;
    volatile int32_t        jointhr;
    volatile int32_t        sigpend;
    volatile int32_t        sigmask;
    volatile int32_t       *statptr;
    volatile int32_t        waitchan;
    volatile int32_t        genregs[V0_INTEGER_REGISTERS];
    volatile int32_t        sysregs[V0_SYSTEM_REGISTERS];
    volatile struct v0seg   segtab[V0_SEGMENT_ENTRIES];
    volatile fxp_t          fxp64regs[V0_FXP_REGISTERS];
    volatile double         fpu64regs[V0_FPU_REGISTERS];
    volatile float          dsp32regs[V0_DSP_REGISTERS];
};

#endif /* __MACH_V0_TYPES_H__ */

