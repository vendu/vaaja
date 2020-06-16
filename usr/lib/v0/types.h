#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>

/* assembler types */
typedef int32_t                 m_reg_t;
typedef uint32_t                m_ureg_t;
typedef int32_t     			m_word_t;
typedef uint32_t    			m_uword_t;
typedef int16_t     			m_half_t;
typedef uint16_t    			m_uhalf_t;
typedef int8_t      			m_byte_t;
typedef uint8_t     			m_ubyte_t;
typedef int64_t                 m_wide_t;
typedef uint64_t                m_uwide_t;
typedef int32_t                 m_adr_t;
typedef uint32_t                m_uadr_t;

/* virtual machine types */

struct v0romparm {
    m_adr_t     buf;
    m_uword_t   bufsize;
};

struct v0trapctx {
    m_reg_t     sp;
    m_reg_t     stksize;
};

struct v0uctx {
    m_reg_t oldfp;
    m_reg_t retadr;
    m_reg_t r1_5[5];
    m_reg_t rx0_rx15[16];
    m_reg_t r12_r15[4];     // FP, SP, PC, LR
    /* 416 bytes reserved for coprocessor use */
};

#endif /* __V0_TYPES_H__ */

