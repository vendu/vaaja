#ifndef __STDDEF_H__
#define __STDDEF_H__

#if !defined(NULL)
#define NULL            ((void *)0)
#endif
#if !defined(__size_t_defined)
typedef uintptr_t       size_t;
#endif
typedef intptr_t        ssize_t;
#if !defined(__time_t_defined)
typedef
#endif
#if !defined(__ptrdiff_t_defined)
typedef intptr_t        ptrdiff_t;
#endif
#if !defined(__wchar_t_defined)
typedef uint32_t        wchar_t;
#endif

#if (_ISOC11_SOURCE)
typedef long long       max_align_t;
#endif

#define offsetof(t, m) ((size_t)(&((t *)0)->m))

#endif /* __STDDEF_H__ */

