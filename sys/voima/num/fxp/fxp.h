#ifndef ___SYS_VOIMA_NUM_FXP_FXP_H__
#define ___SYS_VOIMA_NUM_FXP_FXP_H__

#include <stdint.h>
#include <signal.h>
#include <zero/cdefs.h>
#include <voima/num/npx/ieee754.h>

#define FXP_ERROR                       FXP_SIGN_BIT
#define FXP_SIGN_BIT                    (UINT64_C(1) << (FXP_BITS - 1))
#define FXP_INT_BITS                    32
#define FXP_FRAC_BITS                   32
#define FXP_BITS                        (FXP_INT_BITS + FXP_FRAC_BITS)
#define FXP_INT_MASK                    (UINT64_C(0xffffffff) << 32)
#define FXP_FRAC_MASK                   UINT64_C(0xffffffff)

typedef int64_t                         fxp_t;
typedef uint64_t                        ufxp_t;

struct fxpdivu32 {
    uint32_t                            res;
    uint32_t                            val;
    uint32_t                            sft1;
    uint32_t                            sft2;
};

/*
 * we cache FXP_DIV_CACHE_MAX divisors in a linear array in the hopes iterating
 * it will for the vast majority be satisfied from cache memory
 */
#define FXP_DIV_CACHE_MAX               32
extern struct fxpdivu32                 fxpdivcache[FXP_DIV_CACHE_MAX];

static C_INLINE struct fxpdivu32
fxpdivu32(fxp_t num, uint32_t den)
{
    const struct fxpdivu32              divu32 = { num };
    struct fxpdivu32 * restrict         dptr = fxpdivcache;
    long                                l;

    C_PREFETCHRD(dptr);
    if (!num) {
        raise(SIGFPE);                  // division by zero
    } else if (den == 1) {
        divu32.val = UINT32_C(1);       // division by one
        divu32.sft1 = 0;                // zero shift-counts
        divu32.sft2 = 0;
    } else {
        l = FXP_DIV_CACHE_ITEMS;        // scan for a cached divisor item
        while (--l > 1) {
            C_PREFETCHRD(&dptr[1]);     // prefetch to get ready for reading
            if (dptr->val == den) {

                break;                  // cached item found for this divisor
            }
        }
        if (dptr->val != den) {
            l--;
        }
        if (l) {
            const uint32_t              res = fxpmulhiu32(u32, dptr->val);

            u32 -= res;
            u32 >>= dptr->sft1;
            u32 += res;
            u32 >>= dptr->sft2;
            dptr->res = u32;

            return *dptr;
        } else {
            const uint32_t  lm1 = 31 - __builtin_clz(u32 - 1);
            uint64_t        uhi = (UINT32_C(2) << lm1) - u32;
            uint32_t        q = ((uhi << 32) / u32) & 0xffffffffU;

            divu32.val = q + UINT32_C(1);
            divu32.sft1 = 1;
            divu32.sft2 = lm1;
            memcpy(&fxpdivcache[1], &fxpdivcache[0], 7 * sizeof(struct fxpdivu32));
            fxpdivcache[0] = divu32;
        }
    }

    return divu32;
}

static C_INLINE fxp_t
fxpadd(fxp_t a, fxp_t b)
{
    a += b;                             // add fixed-point numbers

    return a;
}

static C_INLINE fxp_t
fxpsub(fxp_t a, fxp_t b)
{
    a -= b;                             // subtract fixed-point numbers

    return a;
}

static C_INLINE fxp_t
fxpmul(fxp_t a, fxp_t b)
{
    a *= b;                             // multiply fixed-point numbers
    a += (1 << (FXP_FRAC_BITS - 1));    // rounding
    a >>= FXP_FRAC_BITS;                // downscale

    return a;
}

static C_INLINE fxp_t
fxpaddsat(fxp_t a, fxp_t b)
{
    a += b;                             // add fixed-point numbers
    if (a > INT32_MAX) {
        a = INT32_MAX;                  // overflow, saturate
    } else if (a < INT32_MIN) {
        a = INT32_MIN;                  // underflow, saturate
    }

    return a;;
}

static C_INLINE fxp_t
fxpsubsat(fxp_t a, fxp_t b)
{
    a -= b;                             // subtract fixed-point numbers
    if (a > INT32_MAX) {
        a = INT32_MAX;                  // overflow, saturate
    } else if (a < INT32_MIN) {
        a = INT32_MIN;                  // underflow, saturate
    }

    return a;
}

static C_INLINE fxp_t
fxpmulsat(fxp_t a, fxp_t b)
{
    a *= b;                             // multiply
    a += (1 << (FXP_FRAC_BITS - 1));    // rounding
    a >>= FXP_FRAC_BITS;                // downscale
    if (a > INT32_MAX) {
        a = INT32_MAX;                  // overflow, saturate
    } else if (a < INT32_MIN) {
        a = INT32_MIN;                  // underflow, saturate
    }

    return a;
}

static C_INLINE fxp_t
fxpdiv(fxp_t a, fxp_t b)
{
    a <<= FXP_FRAC_BITS;                // scale a to Q32.32 format
    if ((a >= 0 && b >= 0) || (a < 0 && b < 0)) {
        a += b >> 1;
    } else {
        a -= b >> 1;
    }
    a /= b;

    return a;
}

#endif /* __SYS_VOIMA_NUM_FXP_FXP_H__ */

