#ifndef __MATH_FXPC32_H__
#define __MATH_FXPC32_H__

/*
 * velho implementation of the CORDIC-algorithm
 * REFERENCE: https://www.drdobbs.com/database/implementing-cordic-algorithms/184408428
 */

#define FXPC32_BITS         32
#define FXPC32_SIGN_BIT     (INT32_C(1) << 31)
#define FXPC32_INT_BITS     (FXPC32_BITS - 1 - FXPC32_FRAC_BITS)
#define FXPC32_FRAC_BITS    29

typedef int32_t             fxpc32_t;
typedef uint32_t            ufxpc32_t;

#endif /* __MATH_FXPC32_H__ */

