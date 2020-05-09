#ifndef __MACH_X86_PROF_H__
#define __MACH_X86_PROF_H__

#include <mach/asm.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * THANKS
 * ------
 * Thomas 'tommycannady' Cannady for help with testing and fixing the MSVC
 * versions of the macros... :)
 */

#include <stdint.h>
#include <zero/cdefs.h>

struct _tickval {
    union {
        uint64_t u64;
        uint32_t u32v[2];
    } u;
};

#if defined(_MSC_VER)

static __inline__ uint64_t
_rdtsc(struct _tickval *tp)
{
    uint64_t ret;

    ret = rdtsc();
    if (tp) {
        tp->u.u64 = ret;
    }

    return ret;
}

static __inline__ uint64_t
_rdpmc(struct _tickval *tp)
{
    uint64_t ret;

    ret = rdpmc();
    if (tp) {
        tp->u.u64 = ret;
    }

    return ret;
}

#else /* !defined(_MSC_VER) */

#if !defined(__x86_64__) && !defined(__amd64__)
static __inline__ uint64_t
_rdtsc(struct _tickval *tp)
{
    uint64_t ret;

    m_membar();
    __asm__ __volatile__ ("rdtsc\n"
                          : "=A" (ret));
    if (tp) {
        tp->u.u64 = ret;
    }

    return ret;
}
#else /* x86-64 */
static __inline__ uint64_t
_rdtsc(struct _tickval *tp)
{
    uint64_t ret;
    uint32_t lo;
    uint32_t hi;

        m_membar();
    __asm__ __volatile__ ("rdtsc\n"
             : "=a" (lo), "=d" (hi));
    ret = hi;
    ret <<= 32;
    ret |= lo;
    if (tp) {
        tp->u.u64 = ret;
    }

    return ret;
}
#endif

/* read performance monitor counter */
static __inline__ uint64_t
_rdpmc(struct _tickval *tp, int id)
{
    uint64_t ret;
    uint32_t lo;
    uint32_t hi;

    __asm__ __volatile__ ("movl %2, %%ecx\n"
                          "rdpmc\n"
                          "mov %%eax, %0\n"
                          "mov %%edx, %1"
                          : "=rm" (lo), "=rm" (hi)
                          : "rm" (id)
                          : "eax", "edx");
    ret = hi;
    ret <<= 32;
    ret |= lo;
    if (tp) {
        tp->u.u64 = ret;
    }

    return ret;
}

#endif

#define PROFDECLTICK(id)                                                \
    struct _tickval __tv##id[2]
#define profinittick(id)                                                \
    (__tv##id[0].u.u64 = __tv##id[1].u.u64 = UINT64_C(0))
//    memset(&__tv##id, 0, sizeof(__tv##id))
#define profstarttick(id)                                               \
    _rdtsc(&__tv##id[0])
#define profstoptick(id)                                                \
    _rdtsc(&__tv##id[1])
#define proftickdiff(id)                                                \
    (__tv##id[1].u.u64 - __tv##id[0].u.u64)

#if defined(__cplusplus)
}
#endif

#endif /* __MACH_X86_PROF_H__ */

