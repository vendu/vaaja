#ifndef __MACH_IA32_TAGPTR_H__
#define __MACH_IA32_TAGPTR_H__

#include <stddef.h>
#include <stdint.h>
#include <mach/asm.h>

/* pointer in the high-order 64 bits, tag [counter] in the low */
#define TAGPTR_T     uint64_t
#define TAGPTR_ADR_T void *
#define TAGPTR_TAG_T uint32_t

struct tagptr {
    uint32_t cnt;
    uint32_t adr;
};

#define tagptrcmpswap(tp, want, src)                                    \
    m_cmpswapdbl((volatile long *)tp,                                   \
                 (volatile long *)want,                                 \
                 (volatile long *)src)

/* tp = { 0, adr } */
#define tagptrsetadr(adr, tp)                                          \
    ((tp) = (uint64_t)(adr) << 32)
#if 0
/* dest.tag = src.tag */
#define tagptrgettag(tp)                                                \
    ((uint32_t)((tp) & 0xffffffffU))
#define tagptrsettag(tag, tp)                                           \
    ((tp) |= (tag))
/* tag.adr */
#define tagptrgetadr(tp)                                                \
    ((void *)((tp) >> 32))
#endif

static __inline__ long
tagptrcmp(TAGPTR_T *tp1, TAGPTR_T *tp2)
{
    TAGPTR_T diff = *tp1 - *tp2;

    return (diff == 0);
}

#endif /* __MACH_IA32_TAGPTR_H__ */

