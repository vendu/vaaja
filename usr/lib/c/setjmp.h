#ifndef __SETJMP_H__
#define __SETJMP_H__

#if defined(__VELHOLIBC__)

#include <bits/setjmp.h>

#if (_POSIX_C_SOURCE) || (_XOPEN_SOURCE)
typedef struct _jmpbuf  sigjmp_buf[1];
#endif

/* ISO C prototypes. */
ASMLINK NOINLINE int    setjmp(jmp_buf env);
ASMLINK NORETURN void   longjmp(jmp_buf env, int val);

/* Unix prototypes. */
ASMLINK NOINLINE int    _setjmp(jmp_buf env);
ASMLINK NORETURN void   _longjmp(jmp_buf env, int val);

#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)
/* POSIX prototypes */
ASMLINK NOINLINE int    sigsetjmp(sigjmp_buf env, int savesigs);
ASMLINK NORETURN void   siglongjmp(sigjmp_buf env, int val);
#endif

#endif /* __VELHOLIBC__ */

#endif /* __SETJMP_H__ */

