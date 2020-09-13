#ifndef __ZERO_CDEFS_H__
#define __ZERO_CDEFS_H__

#if defined(__STDC_VERSION__)
#if (__STDC_VERSION__ >= 199901L)
#define C_PRAGMA(x)                     _Pragma(#x)
#define C_IEEE_FP                       __attribute__ (__ieee_fp__)
#define C_FP_STRICT_IEEE                _Pragma(FP_CONTRACT OFF)
#define C_FP_CONTRACT                   _Pragma(FP_CONTRACT ON)
#define C_VLA
#define C_RESTRICT                      restrict
#else
#define C_VLA                           0
#endif /* (__STDC_VERSION__ >= 199901L) */
#endif /* defined(__STDC_VERSION__) */

#if (__STDC_VERSION__ >= 201112L)
#include <stdalign.h>
#define C_STATIC_ASSERT                 _Static_assert
#endif /* __STDC_VERSION__ >= 201112L */
#if !defined(__STDC_NO_THREADS__)
//#include <threads.h>
#define C_THREADLOCAL                   _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
#define C_THREADLOCAL                   __thread
#endif /* !defined(__STDC_NO_THREADS) */

#if defined(__STDC_VERSION__)
#define C_REGISTER                      register
#endif

#if defined(__GNUC__) || defined(__clang__)
#define C_SYSV_ABI                      __attribute__ ((__sysv_abi__))
#define C_FANCY_MATH_387                __attribute__ ((__fancy_math_387__))
#define C_L1CODE                        __attribute__ ((__l1_text__))
#define C_SEGMENT(name)                 __attribute__ ((#name))
#define HAVE_C_PREFETCH
#define C_PREFREAD(a)                   __builtin_prefetch(a, 0)
#define C_PREFWRITE(a)                  __builtin_prefetch(a, 1)
#endif

#if defined(_MSC_VER) || defined(__CC_ARM)
#define C_INLINE                        __forceinline
#define C_FLATTEN                       __attribute__ ((__flatten__))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define C_ARTIFICIAL                    __attribute__ ((__artificial__))
#define C_WEAK                          __attribute__ ((__weak__))
#define C_WEAKREF                       __attribute__ ((__weakref__))
#define C_WEAKREFNAME(name)             __attribute__ ((__weakref__ (#name)))
#define C_ALIAS(name)                   __attribute__ ((__alias__ (#name)))
#define C_WEAKALIAS(name)               __attribute__ ((__weak__, __alias__, (#name)))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define C_GENREGS_ONLY(n)               __attribute__ ((__general_regs_only__ (n)))
#define C_SENTINEL(n)                   __attribute__ ((__sentinel__d (n)))
#define C_FASTCALL                      regparm(3)
#define C_FASTFLOAT                     sseregparm(3)
#define C_ASMLINK                       regparm(0)
#define C_FENTRY_NAME(name)             __attribute__ ((__fentry_name__ (#name)))
#define C_FENTRY_SECTION(name)          __attribute__ ((__fentry_section__ (#name)))
#define C_NO_REORDER                    __attribute__ ((__no_reorder__))
#define C_INTERRUPT                     __attribute__ ((__interrupt__))
#define C_UNREACHABLE                   __builtin_unreachable
#define C_HIDDEN                        __attribute__ ((visibility ("hidden")))
#define C_NO_CALLER_SAVED_REGISTERS     __attribute__ ((__no_caller_saved_registers__))
#else /* defined(__GNUC__) || defined(__clang__) */
#define C_GENREGS_ONLY
#define C_SENTINEL
#define C_FASTCALL
#define C_FASTFLOAT
#define C_ASMLINK
#define C_FENTRY_NAME
#define C_FENTRY_SECTION
#define C_NO_REORDER
#define C_INTERRUPT
#define C_UNREACHABLE
#define C_NO_CALLER_SAVED_REGISTERS
#endif /* defined(__GNUC__) || defined(__clang__) */
#if !defined(C_ALIGNED)
#define C_ALIGNED(a)                    __attribute__ ((__aligned__(a)))
#endif
#define C_CONST        	                __attribute__ ((__const__))
#define C_IMMEDIATE(x)                  __builtin_constant_p(x)
#define C_INLINE       	                __inline__ __attribute__ ((__always_inline__))
#define C_NOINLINE     	                __attribute__ ((__noinline__))
#define C_LOCAL_LABEL(lbl)              __label__   lbl
#define C_LABEL_ADDRESS(lbl)            &&lbl
#define C_NOCLONE     	                __attribute__ ((__noclone__))
#define C_NORETURN     	                __attribute__ ((__noreturn__))
#define C_RETURNSTWICE     	            __attribute__ ((__returns_twice__))
#define C_INDIRECT_BRANCH(type)         __attribute__ ((__indirect_branch__ (#type)))
#define C_FUNCTION_RETURN(type)         __attribute__ ((__function_return__ (#type)))
#define C_INDIRECT_RETURN               __attribute__ ((__indirect_return__))
#define C_NOTHROW                       __attribute__ ((__nothrow__))
#if !defined(USED)
#define C_USED                          __attribute__ ((__used__))
#endif
#if !defined(C_UNUSED)
#define C_UNUSED                        __attribute__ ((__unused__))
#endif
#if !defined(C_DEPRECATED)
#define C_DEPRECATED                    __attribute__ ((__deprecated__))
#endif
#if defined(__GNUC__)
#if !defined(C_POISON)
#define C_POISON(...)                   C_PRAGMA(GCC poison __VA_ARGS__)
#endif /* !defined(C_POISON) */
#endif /* defined(__GNUC__) */
#if !defined(C_ERROR)
#define C_ERROR(msg)                    __attribute__ (__error__ (#msg))
#endif
#if !defined(C_WARNING)
#define C_WARNING(msg)                  __attribute__ (__warning__ (#msg))
#endif
#if !defined(C_SYSTEM_HEADER)
#define C_SYSTEM_HEADER                 C_PRAGMA(GCC system_header)
#endif /* !defined(C_SYSTEM_HEADER) */
#if !defined(C_WARN_UNUSED_RESULT)
#define C_WARN_UNUSED_RESULT            __attribute__ ((__warn_unused_result__))
#endif
#if !defined(C_VERSION_ID)
#define C_VERSION_ID(str)               __attribute__ ((__version_id__ (#str)))
#endif
#if !defined(C_SYMVER)
#define C_SYMVER(str)                   __attribute__ ((__symver__ (#str)))
#endif
#if !defined(C_RESTRICT)
#define C_RESTRICT                      __restrict
#endif
#define C_HOT                           __attribute__ ((__hot__))
#define C_COLD                          __attribute__ ((__cold__))
#define C_PURE                          __attribute__ ((__pure__))
#define C_LEAF                          __attribute__ ((__leaf__))
#define C_NAKED                         __attribute__ ((__naked__))
#if !defined(C_THREADLOCAL)
#define C_THREADLOCAL   __thread
#endif
#define C_LIKELY(x)                     (__builtin_expect(!!(x), 1))
#define C_UNLIKELY(x)                   (__builtin_expect(!!(x), 0))
#endif

#if !defined(C_REGISTER)
#define C_REGISTER
#endif
#if !defined(C_L1CODE)
#define C_L1CODE
#endif
#if !defined(C_INLINE)
#define C_INLINE
#endif
#if !defined(C_INLINE)
#define C_FLATTEN
#endif
#if !defined(C_ARTIFICIAL)
#define C_ARTIFICIAL
#endif
#if !defined(C_WEAK)
#define C_WEAK
#endif
#if !defined(C_ALIAS)
#define C_ALIAS
#endif
#if !defined(C_WEAKALIAS)
#define C_WEAKALIAS
#endif
#if !defined(C_NOINLINE)
#define C_NOINLINE
#endif
#if !defined(C_NORETURN)
#define C_NORETURN
#endif
#if !defined(C_RETURNS_TWICE)
#define C_RETURNS_TWICE
#endif
#if !defined(C_CONST)
#define C_CONST
<#endif
#if !defined(C_USED)
#define C_USED
#endif
#if !defined(C_UNUSED)
#define C_UNUSED
#endif
#if !defined(C_HOT)
#define C_HOT
#endif
#if !defined(C_COLD)
#define C_COLD
#endif
#if !defined(C_PURE)
#define C_PURE
#endif
#if !defined(C_LEAF)
#define C_LEAF
#endif
#if !defined(C_NAKED)
#define C_NAKED
#endif

#define adralign(a, p2)                 (((uintptr_t)(a) + (a) - 1) & -(p2))
#define ptralign(a, p2)                 ((void *)adralign(a, p2))

#if !defined(HAVE_C_PREFETCH)
static C_INLINE C_PREFETCH(void *adr)
{
    struct {
        volatile uint32_t               a;
    } buf;
    buf.a = *(uint32_t *)adr;
}
#define C_PREFREAD(a)                   C_PREFETCH(a)
#define C_PREFWRITE(a)                  C_PREFETCH(a)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define C_SCALAR_STORAGE_ORDER(name)    __attribute__ ((__scalar_storage_order (#name)))
#define C_SCALAR_ORDER_LSB              __attribute__ ((__scalar_storage_order ("little-endian")))
#define C_SCALAR_ORDER_LSB              __attribute__ ((__scalar_storage_order ("big-endian")))
#define C_NO_PLT                        __attribute__ ((__no_plt__))
#define C_STACK_PROTECT                 __attribute__ ((__stack_protect__))
#define C_NO_SPLIT_STACK                __attribute__ ((__no_split_stack__))
#define C_NO_STACK_LIMIT                __attribute__ ((__no_stack_limit__))
#define C_NO_SANITIZE_ADDRESS           __attribute__ ((__no_sanitize_address__))
#define C_IFUNC(resolv)                 __attribute__ ((__ifunc__ (#resolv)))
#define C_TARGET_CLONES(list)           __attribute__ ((__target_clones__ (#list)))
#define C_NONNULL(a, ...)               __attribute__ ((__nonnull__ (a, __VA_ARGS__)))
#define C_RETURNS_NONNULL(a, ...)       __attribute__ ((__returns_nonnull__))
#define C_FORMAT_ARG(n)                 __attribute__ ((__format_arg__ (n)))
#define C_MALLOC                        __attribute__ ((__malloc__))
#define C_ALLOC_SIZE(1, ...)            __attribute__ ((__alloc_size__ (a, __VA_ARGS__)))
#define C_ALLOC_ALIGN(n)                __attribute__ ((__alloc_align__ (n)))
#endif

#endif /* __ZERO_CDEFS_H__ */

