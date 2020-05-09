#ifndef __MATH_CORDIC_H__
#define __MATH_CORDIC_H__

/*
 * velho implementation of the CORDIC-algorithm
 * REFERENCE: https://www.drdobbs.com/database/implementing-cordic-algorithms/184408428
 */

#define CORDIC_BITS         32
#define CORDIC_SIGN_BIT     (UINT64_C(1) << 31)
#define CORDIC_INT_BITS     (CORDIC_BITS - 1 - CORDIC_FRAC_BITS)
#define CORDIC_FRAC_BITS    29

typedef int32_t             fxpc32_t;
typedef uint32_t            ufxpc32_t;

#endif /* __MATH_CORDIC_H__ */

