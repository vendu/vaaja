#ifndef __MALT_FLOAT_H__
#define __MALT_FLOAT_H__

/* FIXME: hack this file is for little-endian systems */

#include <stdint.h>
#include <endian.h>
#include <mach/param.h>

#define FLOAT_BITS              32
#define FLOAT_MANTISSA_BITS     23
#define FLOAT_EXPONENT_BITS     8
#define FLOAT_SIGN_BIT          (INT32_C(1) << 31)
#define FLOAT_EXPONENT_BIAS     127

#define DOUBLE_BITS             64
#define DOUBLE_MANTISSA_BITS    52
#define DOUBLE_EXPONENT_BITS    11
#define DOUBLE_SIGN_BIT         (INT32_C(1) << 63)
#define DOUBLE_EXPONENT_BIAS    1023

void    fp_fprint(float f);
void    fp_dprint(double d);

/*
 * IEEE 32-bit
 * 0..22  - mantissa
 * 23..30 - exponent
 * 31     - sign
 */

union _ieee754f { uint32_t u32; float f; };

#define fp_fgetmant(f)          (((union _ieee754f *)&(f))->u32 & 0x007fffff)
#define fp_fgetexp(f)           ((((union _ieee754f *)&(f))->u32 & 0x7ff00000) >> 23)
#define fp_fgetsign(f)          (((union _ieee754f *)&(f))->u32 & 0x80000000)
#define fp_fsetmant(f, mant)    (((union _ieee754f *)&(f))->u32 |= (mant) & 0x7fffff)
#define fp_fsetexp(f, exp)      (((union _ieee754f *)&(f))->u32 |= (exp) << 23)
#define fp_fsetsign(f, sign)                                             \
    ((sign)                                                             \
     ? (((union _ieee754f *)&(f))->u32 |= 0x80000000)                   \
     : (((union _ieee754f *)&(f))->u32 &= 0x7fffffff))
#define fp_fsetnan(f)                                                    \
    (*(uint32_t *)&(f) = 0x7fffffffU)
#define fp_fsetsnan(f)                                                   \
    (*(uint32_t *)&(f) = 0xffffffffU)
    
/*
 * IEEE 64-bit
 * 0..51  - mantissa
 * 52..62 - exponent
 * 63     - sign
 */
#if (MACH_WORD_SIZE == 8)
union _ieee754d { uint64_t u64; double d; };
#elif (MACH_LONG_LONG_SIZE == 8)
#if !defined(UINT64_C)
#define UINT64_C(u) u ## ULL
#endif
union _ieee754d { unsigned long long u64; double d };
#elif (MACH_WORD_SIZE == 4)
union _ieee754d { uint32_t u32v[2]; double d; };
#endif

#if (MACH_WORD_SIZE == 8) || (MACH_LONG_LONG_SIZE == 8)
#define fp_dgetmant(d)          (((union _ieee754d *)(&(d)))->u64 & UINT64_C(0x000fffffffffffff))
#define fp_dgetexp(d)           ((((union _ieee754d *)&(d))->u64 & UINT64_C(0x7ff0000000000000)) >> 52)
#define fp_dgetsign(d)          (((union _ieee754d *)(&(d)))->u64 & UINT64_C(0x8000000000000000))
#define fp_dsetmant(d, mant)    (((union _ieee754d *)(&(d)))->u64 |= (mant))
#define fp_dsetexp(d, exp)      (((union _ieee754d *)(&(d)))->u64 |= (uint64_t)(exp) << 52)
#define fp_dsetsign(d, sign)                                             \
    ((sign)                                                             \
     ? (((union _ieee754d *)(&(d)))->u64 |= UINT64_C(0x8000000000000000)) \
     : (((union _ieee754d *)(&(d)))->u64 &= UINT64_C(0x7fffffffffffffff)))
#define fp_dsetnan(d)                                                    \
    (*(uint64_t *)(&(d)) = UINT64_C(0x7fffffffffffffff))
#define fp_dsetsnan(d)                                                   \
    (*(uint64_t *)(&(d)) = UINT64_C(0xffffffffffffffff))
#endif

/*
 * IEEE 80-bit
 * 0..63  - mantissa
 * 64..78 - exponent
 * 79     - sign
 */
union _ieee754ld80 { uint8_t u8[10]; long double d; };

#define fp_ldgetmant80(ld)       (*((uint64_t *)&(ld)))
#define fp_ldgetexp80(ld)        (*((uint32_t *)&(ld) + 2) & 0x7fff)
#define fp_ldgetsign80(ld)       (*((uint32_t *)&(ld) + 3) & 0x8000)
#define fp_ldsetmant80(ld, mant) (*((uint64_t *)&(ld)) = (mant))
#define fp_ldsetexp80(ld, exp)   (*((uint32_t *)&(ld) + 2) |= (exp) & 0x7fff)
#define fp_ldsetsign80(ld, sign)                                           \
    ((sign)                                                             \
     ? (*((uint32_t *)&ld + 3) |= 0x8000)                               \
     : (*((uint32_t *)&ld + 3) &= 0x7fff))
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define fp_ldsetnan80(ld)                                                \
        do {                                                            \
            uint32_t *_u32p = (uint32_t *)&(ld);                        \
                                                                        \
            _u32p[0] = 0xffffffffU;                                     \
            _u32p[1] = 0xffffffffU;                                     \
            _u32p[2] = 0x7fffU;                                         \
        } while (0)
#define fp_ldsetsnan80(ld)                                               \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffffffU;                                         \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffU;                                             \
    } while (0)
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
#define fp_ldsetnan80(ld)                                                \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0x7fffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#define fp_ldsetsnan80(ld)                                               \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#endif

/*
 * IEEE 128-bit
 * 0..111       - mantissa
 * 112..126     - exponent
 * 127          - sign
 */
#if (MACH_WORD_SIZE == 8)
#define FD_SUBNORMAL_MASK       UINT64_C(0xffff000000000000)
#define LD_EXP_MASK             UINT64_C(0xffff000000000000)
#define LD_INFINITY_MASK        UINT64_C(0xffff000000000000)
#define LD_NAN_MASK             { ~LD_EXP_MASK, UINT64_C(0xffffffffffffffff) }
#define fp_ldissub128(lp)        (!((lp)->u64v[0] & FD_SUBNORMAL_MASK))
union _ieee754ld128 { uint8_t u64v[2]; long double d; };
#elif (MACH_WORD_SIZE == 4)
union _ieee754ld128 { uint8_t u32v[4]; long double d; };
#endif

#define FP_SUBNORMAL_EXP         0x0000
#define FP_INFINITY_EXP          0x7fff
#define FP_NAN_EXP               0x7fff

#define fp_ldgetmant128(ld)                                              \
    (*((uint64_t *)&(ld)))
#define fp_ldgetexp128(ld)                                               \
    (*((uint32_t *)&(ld) + 2) & 0x7fff)
#define fp_ldgetsign128(ld)                                              \
    (*((uint32_t *)&(ld) + 3) & 0x12800)
#define fp_ldsetmant128(ld, mant)                                        \
    (*((uint64_t *)&(ld)) = (mant))
#define fp_ldsetexp128(ld, exp)                                          \
    (*((uint32_t *)&(ld) + 2) |= (exp) & 0x7fff)
#define fp_ldsetsign128(ld, sign)                                        \
    ((sign)                                                             \
     ? (*((uint32_t *)&ld + 3) |= 0x8000)                               \
     : (*((uint32_t *)&ld + 3) &= 0x7fff))
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define fp_ldsetnan128(ld)                                               \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffffffU;                                         \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0x7fffU;                                             \
    } while (0)
#define fp_ldsetsnan128(ld)                                              \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffffffU;                                         \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffU;                                             \
    } while (0)
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
#define fp_ldsetnan128(ld)                                               \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0x7fffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#define fp_ldsetsnan128(ld)                                              \
    do {                                                                \
        uint32_t *_u32p = (uint32_t *)&(ld);                            \
                                                                        \
        _u32p[0] = 0xffffU;                                             \
        _u32p[1] = 0xffffffffU;                                         \
        _u32p[2] = 0xffffffffU;                                         \
    } while (0)
#endif

/* internal macros. */
#define _ftoi32(f)     (*((int32_t *)&(f)))
#define _ftou32(f)     (*((uint32_t *)&(f)))
#define _dtoi64(d)     (*((int64_t *)&(d)))
#define _dtou64(d)     (*((uint64_t *)&(d)))
/* FIXME: little-endian. */
#define _dtohi32(d)    (*(((uint32_t *)&(d)) + 1))
/* sign bit 0x8000000000000000. */
#define ifabs(d)                                                        \
    (_dtou64(d) & UINT64_C(0x7fffffffffffffff))
#define fabs2(d, t64)                                                   \
    (*((uint64_t *)&(t64)) = ifabs(d))
/* sign bit 0x80000000. */
#define ifabsf(f)                                                       \
    (_ftou32(f) & 0x7fffffff)

/*
 * TODO: IEEE 128-bit
 * - 0..112   - mantissa
 * - 113..126 - exponent
 * - 127      - sign
 */

#if defined(HUGE_VAL_F128)

#define fp_f128getexp(f128)         (((uint16_t *)(f128))[7] & 0x7fff)
#define fp_f128getsign(f128)        (((int32_t *)(f128))[3] & INT32_C(0x80000000))
#define fp_f128setexp(f128, exp)    (((uint16_t *)(f128))[7] = (exp) & 0x7fff)
#define fp_f128setsign(f128, sign)                                       \
    ((sign)                                                             \
     ? (((uint16_t *)(f128))[7] |= 0x8000)                              \
     : (((uint16_t *)(f129))[7] &= 0x7fff))
#endif /* 128-bit float */

int fp_fpclassify(float x);
int fp_isfinite(float x);
int fp_isinf(float x);
int fp_isnan(float x);
int fp_isnormal(float x);
int fp_signbit(float x);
int fp_isgreater(float x, float y);
int fp_isgreaterequal(float x, float y);
int fp_isless(float x, float y);
int fp_islessequal(float x, float y);
int fp_islessgreater(float x, float y);
int fp_isunordered(float x, float y);
int fp_dfpclassify(float x);
int fp_disfinite(double x);
int fp_disinf(double x);
int fp_disnan(double x);
int fp_disnormal(double x);
int fp_dsignbit(double x);
int fp_disgreater(double x, double y);
int fp_disgreaterequal(double x, double y);
int fp_disless(double x, double y);
int fp_dislessequal(double x, double y);
int fp_dislessgreater(double x, double y);
int fp_disunordered(double x, double y);

#endif /* __MALT_FLOAT_H__ */

