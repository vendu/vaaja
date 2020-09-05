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
    void                   *buf;
    m_size_t                bufsize;
};

struct v0uframe {
    int32_t                 oldfp;
    int32_t                 retadr;
    int32_t                 r1_5[5];
    int32_t                 rx0_rx15[16];
    int32_t                 r12_r15[4];     // FP, SP, PC, LR
    /* 416 bytes reserved for coprocessor use */
};

struct v0thr {
    volatile int32_t        genregs[V0_INTEGER_REGISTERS];
    volatile int32_t        sysregs[V0_SYSTEM_REGISTERS];
    volatile int64_t        segregs[V0_SEGMENT_REGISTERS];
};

#endif /* __MACH_V0_TYPES_H__ */

