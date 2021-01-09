#ifndef __MACH_ATOMIC_H__
#define __MACH_ATOMIC_H__

#include <stdint.h>
#include <mach/param.h>

/*
 * NOTES
 * -----
 * - m_atomic_t must be big-enough to hold pointer values
 */

/* atomic types */
#if (MACH_WORD_SIZE == 4)
typedef int32_t         m_atomic_t;
#else
typedef int64_t         m_atomic_t;
#endif
typedef int8_t          m_atomic8_t;
typedef uint8_t         m_atomicu8_t;
typedef int16_t         m_atomic16_t;
typedef uint16_t        m_atomicu16_t;
typedef int32_t         m_atomic32_t;
typedef uint32_t        m_atomicu32_t;
typedef int64_t         m_atomic64_t;
typedef uint64_t        m_atomicu64_t;
typedef void           *m_atomicptr_t;
typedef int8_t         *m_atomicptr8_t;
typedef uintptr_t       m_atomicadr_t;

/* for systems without GNU C, we define these in e.g. <mach/ia32/asm.h> */
#if defined(__GNUC__) || defined(__clang__)
/* stack frames */
#define m_atomread(a)      (*((typeof(a))(a)))
#define m_atomwrite(a, v)  (*((typeof(v) *)(a)) = (v))
#define m_syncread(a, res)                                              \
    do {                                                                \
        m_memwrbar();                                                   \
        (res) = m_atomread(a);                                          \
    } while (0)
#define m_syncwrite(a, val)                                             \
    do {                                                                \
        m_memwrbar();                                                   \
        m_atomwrite(a, val);                                            \
    } while (0)
#endif

#endif /* __MACH_ATOMIC_H__ */

