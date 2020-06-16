#include <stdint.h>
#include <zero/cdefs.h>
#include <math.h>
#include <malt/irp.h>
#include <malt/fxp64.h>
#if defined(TEST_FXP64)
#include <stdio.h>
#include <stdlib.h>
#endif

static fxp64_t  g_fxp64irpval;

void
fxp64print(fxp64_t fxp)
{
    printf("%s", (fxp & FXP64_SIGN_BIT) ? "-" : "+");
    printf("%x", (int)(fxp >> 32));
    printf("%x\n", (int)(fxp & 0xffffffff));

    return;
}

fxp64_t
double2fxp64(double x)
{
    fxp64_t     fxp = 0; // fixed-point value
    fxp64_t     lim;
    ufxp64_t    bit;
    int         i;
    double      d;

    /* initialize fxp to integer part */
    if (x > 0.0) {
        lim = (fxp64_t)floor(x);
        fxp = lim << FXP64_FRAC_BITS;
    } else if (x < 0.0) {
        lim = (fxp64_t)ceil(x);
        fxp = lim << FXP64_FRAC_BITS;
    } else {

        return 0;
    }
    x -= lim;
    d = 0.5;
    bit = INT64_C(1) << (FXP64_FRAC_BITS - 1);
    for (i = 0 ; i < FXP64_FRAC_BITS ; i++) {
        if (x - d > 0) {
            x -= d;
            fxp |= bit;
        }
        bit >>= 1;
        d *= 0.5;
    }

    return fxp;
}

void
fxp64init(void)
{
    g_fxp64irpval = double2fxp64(1.4256);

    return;
}

fxp64_t
fxp64guessirp(fxp64_t den, fxp64_t *retv)
{
    int     nlz = __builtin_clzll(den);
    int     cnt = FXP64_BITS - FXP64_FRAC_BITS;
    fxp64_t val = INT64_C(3) << (FXP64_FRAC_BITS - 2);    // 0.75
    fxp64_t u0;

    /* normalize abs(den) to [0.5, 1) */
    if (nlz < cnt) {
        cnt -= nlz;
        den >>= cnt;
    } else if (nlz > cnt) {
        cnt = nlz - cnt;
        den <<= cnt;
    }
    *retv = den;
    u0 = (fxp64sub(den, val) << 1) + g_fxp64irpval - den;
    fprintf(stderr, "GUESS: %llx\n", (long long)u0);

    return u0;
}

fxp64_t
fxp64nrpiter1(fxp64_t u, fxp64_t v)
{
    fxp64_t c = INT64_C(1) << FXP64_FRAC_BITS;
    fxp64_t uv = fxp64mul(u, v);
    fxp64_t val = c - uv;

    u *= val;
    fprintf(stderr, "U: %llx\n", (long long)u);

    return u;
}

fxp64_t
fxp64nrpiter2(fxp64_t u, fxp64_t v)
{
    fxp64_t uv = fxp64mul(u, v);
    fxp64_t val = INT64_C(1) << (FXP64_FRAC_BITS - 1);  // 0.5

    u <<= 3;
    val -= uv;
    u *= val;
    fprintf(stderr, "U: %llx\n", (long long)u);

    return u;
}

fxp64_t
fxp64nrpiter3(fxp64_t u, fxp64_t v)
{
    fxp64_t uv = fxp64mul(u, v);
    fxp64_t val = INT64_C(1) << (FXP64_FRAC_BITS - 2);  // 0.25

    u <<= 4;
    val -= uv;
    u *= val;
    fprintf(stderr, "U: %llx\n", (long long)u);

    return u;
}

fxp64_t
fxp64nrpiter4(fxp64_t u, fxp64_t v)
{
    fxp64_t uv = fxp64mul(u, v);
    fxp64_t val = INT64_C(1) << (FXP64_FRAC_BITS - 3);  // 0.125

    u <<= 5;
    val -= uv;
    u *= val;
    fprintf(stderr, "U: %llx\n", (long long)u);

    return u;
}

#if defined(TEST_FXP64)

int
main(int argc, char *argv[])
{
    fxp64_t fxp = INT64_C(65535) << FXP64_FRAC_BITS;
    fxp64_t fxd = INT64_C(37) << FXP64_FRAC_BITS;
    fxp64_t v;
    fxp64_t u;

    fxp64init();
    u = fxp64guessirp(fxd, &v);
    u = fxp64nrpiter1(u, v);
    u = fxp64nrpiter2(u, v);
    u = fxp64nrpiter3(u, v);
    u = fxp64nrpiter4(u, v);
    //    u = fxp64nrpiter(u, v);
    fxp *= u;
    //    fxp >>= FXP64_FRAC_BITS;
    fxp64print(fxp);
    fxp = INT64_C(65535) << FXP64_FRAC_BITS;
    fxp = fxp64lomul(fxp, fxd);
    fprintf(stderr, "LOMUL: %llx (must be %llx)\n",
            (long long)fxp,
            (long long)(65535 * 37) << FXP64_FRAC_BITS);
    fxp64print(fxp);
    fxp = INT64_C(65535) << FXP64_FRAC_BITS;
    fxp = fxp64mul(fxp, fxd);
    fprintf(stderr, "MUL: %llx (must be %llx)\n",
            (long long)fxp,
            (long long)(65535 * 37) << FXP64_FRAC_BITS);
    fxp64print(fxp);
    fxp = INT64_C(65535) << FXP64_FRAC_BITS;
    fprintf(stderr, "IDIV: %llx (must be %x)\n",
            (long long)fxp64idiv(fxp, fxd),
            65535 / 37);
    fxp = INT64_C(1) << FXP64_FRAC_BITS;
    fxp = fxp64div(fxp, fxd);
    fprintf(stderr, "DIV: %llx (must be %llx)\n",
            (long long)fxp, (long long)(65535 / 37));
    fxp64print(fxp);

    exit(0);
}

#endif /* TEST_FXP64 */

