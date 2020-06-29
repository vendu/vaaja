#ifndef __V0_TYPES_H__
#define __V0_TYPES_H__

#include <stdint.h>
#include <v0/regs.h>

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

/* other system types */
typedef uint32_t                m_page_t;
typedef uint32_t                m_size_t;
typedef int32_t                 m_ssize_t;
typedef int32_t                 m_mode_t;

/* virtual machine types */

struct v0romparm {
    m_adr_t     buf;
    m_uword_t   bufsize;
};

struct v0uframe {
    m_reg_t oldfp;
    m_reg_t retadr;
    m_reg_t r1_5[5];
    m_reg_t rx0_rx15[16];
    m_reg_t r12_r15[4];     // FP, SP, PC, LR
    /* 416 bytes reserved for coprocessor use */
};

struct m_thr {
    volatile m_word_t   genregs[V0_GENERAL_REGISTERS];
    volatile m_word_t   sysregs[V0_GENERAL_REGISTERS];
    volatile m_word_t   segregs[V0_SEGMENT_REGISTERS];
};

#endif /* __V0_TYPES_H__ */

