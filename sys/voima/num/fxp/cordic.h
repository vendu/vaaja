#ifndef __SYS_VOIMA_NUM_FXP_CORDIC_H__
#define __SYS_VOIMA_NUM_FXP_CORDIC_H__

#define VOIMA_FXP_CORDIC_ERROR          VOIMA_FXP_CORDIC_SIGN_BIT

#define VOIMA_FXP_CORDIC_BITS           32
#define VOIMA_FXP_CORDIC_SIGN_BIT       (INT32_C(1)                     \
                                         << (VOIMA_FXP_CORDIC_BITS - 1))
#define VOIMA_FXP_CORDIC_INT_BITS       3
#define VOIMA_FXP_CORDIC_FRAC_BITS      29
#define VOIMA_FXP_CORDIC_INT_MASK                                       \
    (((INT32_C(1) << VOIMA_FXP_CORDIC_INT_BITS + 1) - 1)                \
     << VOIMA_FXP_CORDIC_FRAC_BITS)
#define VOIMA_FXP_FRAC_MASK                                             \
    ((INT32_C(1) << VOIMA_FXP_FRAC_BITS) - 1)

typedef int32_t                         voimafxpcordic_t;
typedef uint32_t                        voimafxpucordic_t;

#endif /* __SYS_VOIMA_NUM_FXP_CORDIC_H__ */

