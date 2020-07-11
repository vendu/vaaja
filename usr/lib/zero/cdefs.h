#ifndef __ZERO_CDEFS_H__
#define __ZERO_CDEFS_H__

#if defined(__STDC_VERSION__)
#if (__STDC_VERSION__ >= 199901L)
#   define C_VLA
#   define C_RESTRICT   restrict
#else
#   define C_VLA        0
#endif
#if defined(__GNUC__) || defined(__clang__)
#define C_THREADLOCAL   __thread
#elif (__STDC_VERSION__ >= 201112L)
#include <stdalign.h>
#if !defined(__STDC_NO_THREADS__)
#include <threads.h>
#define C_THREADLOCAL   _Thread_local
#endif /* !defined(__STDC_NO_THREADS__) */
#endif /* __STDC_VERSION__ >= 201112L */
#endif /* __STDC_VERSION__ */

#if defined(_MSC_VER) || defined(__CC_ARM)
#define C_INLINE        __forceinline
#endif

#if defined(__GNUC__) || defined(__clang__)
#if (defined(__i386__) || defined(__i486__)                             \
     || defined(__i586__) || defined(__i686__))
#define C_FASTCALL      regparm(3)
#define C_ASMLINK       regparm(0)
#else
#define C_FASTCALL
#define C_ASMLINK
#endif
#if !defined(C_ALIGNED)
#define C_ALIGNED(a)    __attribute__ ((__aligned__(a)))
#endif
#define C_IMMEDIATE(x)  __builtin_constant_p(x)
#define C_INLINE       	__inline__ __attribute__ ((__always_inline__))
#define C_NOINLINE     	__attribute__ ((__noinline__))
#define C_NORETURN     	__attribute__ ((__noreturn__))
#define C_CONST        	__attribute__ ((__const__))
#if !defined(UNUSED)
#define C_UNUSED        __attribute__ ((__unused__))
#endif
#if !defined(C_RESTRICT)
#define C_RESTRICT      __restrict
#endif
#define C_PURE          __attribute__ ((__pure__))
#if !defined(C_THREADLOCAL)
#define C_THREADLOCAL   __thread
#endif
#define C_LIKELY(x)     (__builtin_expect(!!(x), 1))
#define C_UNLIKELY(x)   (__builtin_expect(!!(x), 0))
#endif

#if !defined(C_INLINE)
#define C_INLINE
#endif
#if !defined(C_NOINLINE)
#define C_NOINLINE
#endif
#if !defined(C_NORETURN)
#define C_NORETURN
#endif
#if !defined(C_CONST)
#define C_CONST
#endif
#if !defined(C_UNUSED)
#define C_UNUSED
#endif
#if !defined(C_PURE)
#define C_PURE
#endif

#define adralign(a, b2) ((uintptr_t)(a) & -(b2))
#define ptralign(a, b2) ((void *)adralign(a, b2))

#endif /* __ZERO_CDEFS_H__ */

