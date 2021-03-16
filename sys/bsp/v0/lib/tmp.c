#if 0
/* Goldschmidt division */
int32_t
gsdiv(int32_t num, int32_t den)
{
    int     sft;
    double  d;
    double  n;
    double  f;

    if (den == 0) {
        /* 0-divisor */
        abort();
    } else if (den > num) {
        /* divisor > nominator */

        return 0;
    } else if (den == num) {
        /* divisor == nominator */

        return 1;
    }
    n = num;
    f = fpirp64((double)den);
    sft = DOUBLE_MANTISSA_BITS + 1 - __builtin_clz(den);
    d = den << sft;
    fp_dsetexp(d, DOUBLE_EXPONENT_BIAS - sft);
    n *= f;
    d *= f;
    f = 2.0 - d;
    num = (int32_t)n;

    return num;
}
#endif

