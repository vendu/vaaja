#ifndef __VARARGS_H__
#define __VARARGS_H__

#if !defined(__zen__)

/* implementation of old-style Unix <varargs.h> using later <stdarg.h> */

#include <stdint.h>
#include <stdarg.h>
#include <endian.h>

#if defined(__GNUC__)
#define va_alist      __builtin_va_alist
#define __va_ellipsis ...
#endif

#undef va_list
#undef va_start
#undef va_end
#undef __va_argsiz
#undef va_arg
#define va_dcl        int va_alist; __va_ellipsis
#define va_start(v)   ((v) = (va_list)&va_alist)
#define va_end        ((void)0)
#define __va_argsiz(t)                                                   \
    (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define va_arg(v, t)  ((v) = (int8_t *)(v) + __va_argsiz(t),             \
                       *((t *)(void *)((int8_t *)(v) - __va_argsiz(t))))
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define va_arg(v, t)  ((v) = ((int8_t *)(v) - ((sizeof(t) < __va_argsiz(t)), \
                                               ? sizeof(t)              \
                                               : __va_argsiz(type))))
#endif /* __BYTE_ORDER__ */

#endif /* !__zen__ */

#endif /* __VARARGS_H__ */

