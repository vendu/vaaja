#ifndef __STDARG_H__
#define __STDARG_H__

#include <stdlib.h>
#include <stdint.h>

#if defined(__zen__)
#include <sys/zen/malloc.h>
#define MALLOC(n) kmalloc(n)
#define FREE(ptr) kfree(ptr)
#else
#define MALLOC(n) malloc(n)
#define FREE(ptr) free(ptr)
#endif

struct __valist {
    int32_t        gpregofs;
    int32_t        fpregofs;
    union {
        uintptr_t  ofs;
        int8_t    *args;
    } stk;
    int8_t        *regsave;
};

typedef struct __valist *va_list;

#if defined(__GNUC__)
#define va_start(ap, last)                                              \
    ((ap) = (va_list)MALLOC(sizeof(struct __valist)),                   \
     *(ap) = *(struct __valist *)((int8_t *)__builtin_frame_address(0)  \
                                  - 16),                                \
     (ap)->stk.args  = ((int8_t *)__builtin_frame_address(0)            \
                        + (ap)->stk.ofs),                               \
     (ap)->regsave = (int8_t *)__builtin_frame_address(0) -  176 - 16)
#endif

#define va_copy(dest, src)                                              \
    ((dest)[0].gpregofs = src[0].gpregofs,                              \
     (dest)[0].fpregofs = src[0].fpregofs,                              \
     (dest)[0].stk.args = src[0].stk.args,                              \
     (dest)[0].regsave = src[0].regsave)
    //    ((dest) = (src))
#define va_end(ap) FREE(ap)

#if defined(_MSC_VER) && !__INTEL_COMPILER
#define VA_MSVC_HACK(func, args) func args
#define VA_MSVC_APPLY(func, ...) VA_MSVC_HACK(func, (__VA_ARGS__))
#define VA_LENGTH(...)           VA_MSVC_APPLY(VA_LENGTH_, 0, ## __VA_ARGS__, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x02, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00)
#else
#define VA_LENGTH(...)           VA_LENGTH_(0, ## __VA_ARGS__, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02,  0x01, 0x00)
#endif

#define VA_LENGTH_(_0x00, _0x01, _0x02, _0x03, _0x04, _0x05, _0x06, _0x07, _0x08, _0x09, _0x0a, _0x0b, _0x0c, _0x0d, _0x0e, _0x0f, N, ...) N
#define VAFUNC(func, ...) func(VA_LENGTH(__VA_ARGS__), __VA_ARGS__)

#if defined(__GNUC__) && 0
#if !defined(__GNUC_VA_LIST)
typedef __builtin_va_list __gnuc_va_list;
#endif
typedef __gnuc_va_list    va_list;
#define va_start(v, l)    __builtin_va_start(v, l)
#define va_end(v)         __builtin_va_end(v)
#if 0
#define va_arg(v, l)      __builtin_va_arg(v, l)
#endif
#endif
#if defined(__STDC_VERSION) && (__STDC_VERSION__ >= 199901L)
/* C99 addition */
#define va_copy(d, s)     __builtin_va_copy(d, s)
#elif defined(_MSC_VER)
#include <va_list.h>
/* courtesy of Microsoft */
#define _INTSIZEOF(n)	  ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap,v)	  (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap,t)	  (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)	  (ap = (va_list)0)
#elif defined(__x86_64__) || defined(__amd64__)
#include <x86-64/stdarg.h>
#elif ((defined(__i386__) || defined(__i486__)                          \
        || defined(__i386__) || defined(__i486__)))
#include <ia32/stdarg.h>
#endif /* compiler */

#if defined(__VARARGS_H__)
/* remove <varargs.h> functionality; thanks to Microsoft for this idea */
#undef va_alist
#undef va_dcl
#undef va_start
#undef va_end
#undef va_arg
#endif /* __VARARGS_H__ */

#endif /* __STDARG_H__ */

