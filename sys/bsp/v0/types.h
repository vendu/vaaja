#ifndef MACH_V0_TYPES_H
#define MACH_V0_TYPES_H

#include <mach/param.h>

typedef uint32_t                m_intrdesc_t;
typedef uint32_t                m_pagedesc_t;
typedef int32_t                 m_filedesc_t;
typedef int32_t                 m_reg_t;
typedef uint32_t                m_ureg_t;
typedef int32_t                 m_word_t;
typedef uint32_t                m_uword_t;
typedef int16_t                 m_half_t;
typedef uint16_t                m_uhalf_t;
typedef int8_t                  m_byte_t;
typedef uint8_t                 m_ubyte_t;
typedef uint32_t                m_adr_t;
typedef void                   *m_ptr_t;
#if (MACH_LONG_LONG_SIZE == 8)
typedef int64_t                 m_longlong_t;
typedef uint64_t                m_ulonglong_t;
#endif
#if (MACH_LONG_SIZE == 8)
typedef int64_t                 m_long_t;
typedef uint64_t                m_ulong_t;
#endif

struct m_trapframe {
    m_word_t                    pc;
    m_word_t                    msw;
    m_word_t                    usp;
};

#endif /* MACH_V0_TYPES_H */
