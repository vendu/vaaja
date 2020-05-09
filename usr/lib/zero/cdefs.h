#ifndef __ZERO_CDEFS_H__
#define __ZERO_CDEFS_H__

#if defined(__STDC_VERSION__)
#if (__STDC_VERSION__ >= 199901L)
#   define VLA
#   define RESTRICT     restrict
#else
#   define VLA          0
#endif
#if (__STDC_VERSION__ >= 201112L)
#include <stdalign.h>
#if !defined(__STDC_NO_THREADS__)
#include <threads.h>
#define THREADLOCAL     _Thread_local
#endif /* !defined(__STDC_NO_THREADS__) */
#endif /* __STDC_VERSION__ >= 201112L */
#endif /* __STDC_VERSION__ */

#if defined(_MSC_VER) || defined(__CC_ARM)
#define INLINE          __forceinline
#endif

#if defined(__GNUC__) || defined(__clang__)
#if (defined(__i386__) || defined(__i486__)                             \
     || defined(__i586__) || defined(__i686__))
#define ASMLINK         regparm(0)
#else
#define ASMLINK
#endif
#if !defined(ALIGNED)
#define ALIGNED(a)      __attribute__ ((__aligned__(a)))
#endif
#define IMMEDIATE(x)    __builtin_constant_p(x)
#define INLINE       	__inline__ __attribute__ ((__always_inline__))
#define NOINLINE     	__attribute__ ((__noinline__))
#define NORETURN     	__attribute__ ((__noreturn__))
#define CONST        	__attribute__ ((__const__))
#if !defined(UNUSED)
#define UNUSED          __attribute__ ((__unused__))
#endif
#if !defined(RESTRICT)
#define RESTRICT        __restrict
#endif
#define PURE            __attribute__ ((__pure__))
#if !defined(THREADLOCAL)
#define THREADLOCAL     __thread
#endif
#define LIKELY(x)       (__builtin_expect(!!(x), 1))
#define UNLIKELY(x)     (__builtin_expect(!!(x), 0))
#endif

#if !defined(INLINE)
#define INLINE
#endif
#if !defined(NOINLINE)
#define NOINLINE
#endif
#if !defined(NORETURN)
#define NORETURN
#endif
#if !defined(CONST)
#define CONST
#endif
#if !defined(UNUSED)
#define UNUSED
#endif
#if !defined(PURE)
#define PURE
#endif

#define adralign(a, b2) ((uintptr_t)(a) & -(b2))
#define ptralign(a, b2) ((void *)adralign(a, b2))

#endif /* __ZERO_CDEFS_H__ */

