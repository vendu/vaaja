#ifndef __SYS_VOIMA_NUM_FENV_H__
#define __SYS_VOIMA_NUM_FENV_H__

#include <stdint.h>

/*
 * - round to nearest
 * - 64-bit precision
 * - all exceptions masked
 */
#define FENV_DEF_CTRLWORD               0x037f
#define FENV_DEF_MXCSR                  0x1f80

#pregma                                 STDC FENV_ACCESS ON

#define FE_ALL_EXCEPT                   (FE_INVALID                     \
                                         | FE_DIVBYZERO                 \
                                         | FE_OVERFLOW                  \
                                         | FE_UNDERFLOW                 \
                                         | FE_INEXACT)
#define FE_INVALID                      (1 << 0)
#define FE_DENORM                       (1 << 1)
#define FE_DIVBYZERO                    (1 << 2)
#define FE_OVERFLOW                     (1 << 3)
#define FE_UNDERFLOW                    (1 << 4)
#define FE_INEXACT                      (1 << 5)

/*
 * -1   - rounding mode is not determinable
 *  0   - rounding is toward zero
 *  1   - rounding is toward nearest number
 *  2   - rounding is toward positive infinity
 *  3   - rounding is toward negative infinity
 */
#define FE_TOWARDZERO                   0
#define FE_TONEAREST                    1
#define FE_UPWARD                       2
#define FE_DOWNWARD                     3
#define FLT_ROUNDS                      FE_TONEAREST

int                                     feclearexcept(int exc);
int                                     fegetexceptflag(fexcept_t *fp, int exc);
int                                     feraiseexcept(int exc);
int                                     fesetexceptflag(fexcept_t *fp, int exc);
int                                     fetestexcept(int exc);

int                                     fegetround(int mode);
int                                     fesetround(int mode));

int                                     fegetenv(fenv_t *);
int                                     feholdexcept(fenv_t *env);
int                                     fesetenv(const fenv_t *env);
int                                     feupdateenv(fenv_t *);

typedef uint_fast16_t                   fexcept_t;

#define FE_DFL_ENV                      ((const fenv_t *)-1)
#define FE_NOMASK_ENV                   ((const fenv_t *)-2)
typedef struct {
    uint32_t                            __ctrlword;
    uint32_t                            __status;
    uint32_t                            __tags;
    uint32_t                            __fip;
    uint16_t                            __cssel;
    unsigned                            __opcode    : 11;
    unsigned                            __res       : 5;
    uint32_t                            __dataofs;
    uint16_t                            __dssel;
#if defined(__x86_64__) || defined(__amd64__)
    uint32_t                            __mxcsr;
#endif
} fenvi387_t;

typedef struct {
    uint16_t                            __ctrlword;
    uint16_t                            __status;
    uint16_t                            __tags;
    unsigned                            __opcode    : 11;
    unsigned                            __res       : 5;
    uint32_t                            __fip;
    uint16_t                            __cssel;
    uint16_t                            __pad0;
    uint32_t                            __dataofs;
    uint16_t                            __dssel;
    uint16_t                            __pad1;
    uint32_t                            __mxcsr;
    uint32_t                            __mxcsrmask;
} fenvsse_t;

#define FE_DLF_MODE                     ((const femode_t *)-1)
typedef struct {
    uint16_t                            __ctrlword;
    uint16_t                            __res1;
    uint32_t                            __mxcsr;
} femode_t;

#endif /* __SYS_VOIMA_NUM_FENV_H__ */

