#ifndef __STDDEF_H__
#define __STDDEF_H__

#include <stdint.h>
#include <share/size_t.h>

#if !defined(NULL)
#define NULL                    ((void *)0)
#endif
#if !defined(offsetof)
#define offsetof(t, m)          ((size_t)(&((t *)0)->m))
#endif
/* #if !defined(__size_t_defined) */
/* typedef uintptr_t               size_t; */
/* #define __size_t_defined        1 */
/* #endif */
#if !defined(__ssize_t_defined)
typedef intptr_t                ssize_t;
#define __ssize_t_defined       1
#endif
#if !defined(__ptrdiff_t_defined)
typedef intptr_t        ptrdiff_t;
#define __ptrdiff_t_defined     1
#endif
#if !defined(__wchar_t_defined)
typedef uint32_t                wchar_t;
#define __wchar_t_defined       1
#endif

#if defined(_ISOC11_SOURCE)
typedef long long               max_align_t;
#endif

#endif /* __STDDEF_H__ */

