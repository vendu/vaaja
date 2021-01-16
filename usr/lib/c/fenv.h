#ifndef __FENV_H__
#define __FENV_H__

#include <features.h>
#include <stdint.h>
#include <float.h>
#include <zero/cdecl.h>
#include <zero/param.h>

/* exceptions */
#define FE_INEXACT    0x01 /* inexact result */
#define FE_DIVBYZERO  0x02 /* raise(SIGFPE) */
#define FE_UNDERFLOW  0x04 /* result not representable due to underflow */
#define FE_OVERFLOW   0x08 /* result not representable due to overflow */
#define FE_INVALID    0x10 /* invalid operation */
#define FE_DENORMAL   0x20 /* NOT in POSIX.1 */
#define FE_ALL_EXCEPT \
    (FE_INEXACT \
     | FE_DIVBYZERO \
     | FE_UNDERFLOW \
     | FE_OVERFLOW \
     | FE_INVALID \
     | FE_DENORMAL)

#if defined(__x86_64__) || defined(__amd64__) \
    || defined(__i386__) || defined(__i486__) \
    || defined(__i586__) || defined(__i686__)
#include <ia32/fenv.h>
#endif

/* function prototypes from POSIX.1 */
/* exception handling */
extern int feclearexcept(int mask);
extern int fegetexceptflag(fexcept_t *except, int mask);
extern int fesetexceptflag(const fexcept_t *except, int mask);
extern int feraiseexcept(int mask);
extern int feholdexcept(fenv_t *env);
extern int fetestexcept(int mask);
/* rounding mode */
extern int fegetround(void);
extern int fesetround(int mode);
/* floating-point environment management */
extern int fegetenv(fenv_t *env);
extern int fesetenv(const fenv_t *env);
extern int feupdateenv(const fenv_t *env);

#if defined(USEBSD) && (USEBSD)
extern int fegetexcept(void);
extern int fedisableexcept(int mask);
extern int feenableexcept(int mask);
#endif

/* rounding modes */
#if !defined(FE_TOWARDZERO)
#define FE_TOWARDZERO 0
#define FE_TONEAREST  1
#define FE_UPWARD     2
#define FE_DOWNWARD   3
#endif /* !defined(FE_TOWARDZERO) */

extern const fenv_t   __fe_dfl_env;
#define FE_DFL_ENV    (&__fe_dfl_env)

#endif /* __FENV_H__ */

