#include <stdint.h>
#include <zero/cdefs.h>
#include <zero/shuntc.h>

/* C character conversion tables */
static long             shuntcdectab[256];
static long             shuntchextab[256];
static long             shuntcocttab[256];
/* C floating-point conversion tables */
static float            shuntcdecflttab[256];
static float            shuntchexflttab[256];
static float            shuntcoctflttab[256];
static double           shuntcdecdbltab[256];
static double           shuntchexdbltab[256];
static double           shuntcoctdbltab[256];
/* C operation lookup tables (globally visible) */
long                    shuntcopchartab[256];
long                    shuntcopprectab[SHUNTCNTAB];
long                    shuntcopnargtab[SHUNTCNTAB];
shuntcop_t             *shuntcevaltab[SHUNTCNTAB];
/* stacks and queues for parsing and evaluating expressions */
struct shuntcparser {
    struct shuntctoken *operstk;
    struct shuntctoken *operstktop;
    struct shuntctoken *tokenqueue;
    struct shuntctoken *tokentail;
    struct shuntctoken *parsequeue;
    struct shuntctoken *parsetail;
}                       shuntcparser;
/* currently displayed radix */
long                    shuntcradix;

PURE SHUNT_INT
shuntcnot64(struct shuntctoken *arg1, struct shuntctoken *dummy)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res;

    (void)*dummy;
    res = ~src;

    return res;
}

PURE SHUNT_INT
shuntcand64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res &= src;

    return res;
}

PURE SHUNT_INT
shuntcor64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res |= src;

    return res;
}

PURE SHUNT_INT
shuntcxor64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res ^= src;

    return res;
}

PURE SHUNT_INT
shuntcshl64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT cnt = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res <<= cnt;

    return res;
}

PURE SHUNT_INT
shuntcsar64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT cnt = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;
    SHUNT_INT sign = res & UINT64_C(0x8000000000000000);

    sign = (sign & UINT64_C(0xffffffffffffffff)) << (64 - cnt);
    res >>= cnt;
    res |= sign;

    return res;
}

PURE SHUNT_INT
shuntcshr64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT cnt = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;
    SHUNT_INT mask = UINT64_C(0xffffffffffffffff) >> cnt;

    res >>= cnt;
    res &= mask;

    return res;
}

PURE SHUNT_INT
shuntcror64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT cnt = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;
    SHUNT_INT mask = UINT64_C(0xffffffffffffffff) >> (64 - cnt);
    SHUNT_INT bits = res & mask;

    bits <<= 64 - cnt;
    res >>= cnt;
    res |= bits;

    return res;
}

PURE SHUNT_INT
shuntcrol64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT cnt = arg1->data.i64;
    SHUNT_INT res = arg2->data.ui64;
    SHUNT_INT mask = UINT64_C(0xffffffffffffffff) << (64 - cnt);
    SHUNT_INT bits = res & mask;

    bits >>= 64 - cnt;
    res <<= cnt;
    res |= bits;

    return res;
}

PURE SHUNT_INT
shuntcinc64(struct shuntctoken *arg1, struct shuntctoken *dummy)
{
    SHUNT_INT src = arg1->data.i64;

    (void)*dummy;
#if defined(SHUNTSMARTTYPES)
    arg1->flg = 0;
    if (src < SHUNT_INTMAX) {
        src++;
    } else if (arg1->type == SHUNT_INT64) {
        if (arg1->type == SHUNT_USERSIGNED) {
            arg1->flg |= SHUNTCOVERFLOW;
            src++;
        } else {
            src++;
            if (!src) {
                if (!src) {
                    arg1->flg |= SHUNTCZERO;
                }
            } else if (src > SHUNT_INTMAX) {
                arg1->type = SHUNT_UINT64;
                arg1->sign = SHUNT_UNSIGNED;
            }
        }
    } else if (arg1->type == SHUNT_UINT64) {
        if (src == USHUNT_INTMAX) {
            arg1->flg |= SHUNTCOVERFLOW;
        }
        src++;
    }
#else
    src++;
#endif

    return src;
}

PURE SHUNT_INT
shuntcdec64(struct shuntctoken *arg1, struct shuntctoken *dummy)
{
    SHUNT_INT src = arg1->data.i64;

    (void)*dummy;
#if defined(SHUNTSMARTTYPES)
    arg1->flg = 0;
    if (src == INT64_MIN) {
        if (arg1->type == SHUNT_INT64) {
            arg1->flg |= SHUNTCUNDERFLOW;
        }
    } else if (arg1->type == SHUNT_UINT64) {
        if (!src) {
            arg1->flg |= SHUNTCUNDERFLOW;
        }
    }
    src--;
#else
    src--;
#endif

    return src;
}

PURE SHUNT_INT
shuntcadd64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res += src;

    return res;
}

PURE SHUNT_INT
shuntcsub64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res -= src;

    return res;
}

PURE SHUNT_INT
shuntcmul64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res *= src;

    return res;
}

PURE SHUNT_INT
shuntcdiv64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res /= src;

    return res;
}

PURE SHUNT_INT
shuntcmod64(struct shuntctoken *arg1, struct shuntctoken *arg2)
{
    SHUNT_INT src = arg1->data.i64;
    SHUNT_INT res = arg2->data.i64;

    res %= src;

    return res;
}

void
shuntcinitconvtab(void)
{
    /* integral tables */
    shuntcdectab['0'] = 0x00;
    shuntcdectab['1'] = 0x01;
    shuntcdectab['2'] = 0x02;
    shuntcdectab['3'] = 0x03;
    shuntcdectab['4'] = 0x04;
    shuntcdectab['5'] = 0x05;
    shuntcdectab['6'] = 0x06;
    shuntcdectab['7'] = 0x07;
    shuntcdectab['8'] = 0x08;
    shuntcdectab['9'] = 0x09;
    shuntchextab['0'] = 0x00;
    shuntchextab['1'] = 0x01;
    shuntchextab['2'] = 0x02;
    shuntchextab['3'] = 0x03;
    shuntchextab['4'] = 0x04;
    shuntchextab['5'] = 0x05;
    shuntchextab['6'] = 0x06;
    shuntchextab['7'] = 0x07;
    shuntchextab['8'] = 0x08;
    shuntchextab['9'] = 0x09;
    shuntchextab['a'] = 0x0a;
    shuntchextab['b'] = 0x0b;
    shuntchextab['c'] = 0x0c;
    shuntchextab['d'] = 0x0d;
    shuntchextab['e'] = 0x0e;
    shuntchextab['f'] = 0x0f;
    shuntchextab['A'] = 0x0a;
    shuntchextab['B'] = 0x0b;
    shuntchextab['C'] = 0x0c;
    shuntchextab['D'] = 0x0d;
    shuntchextab['E'] = 0x0e;
    shuntchextab['F'] = 0x0f;
    shuntcocttab['0'] = 0x00;
    shuntcocttab['1'] = 0x01;
    shuntcocttab['2'] = 0x02;
    shuntcocttab['3'] = 0x03;
    shuntcocttab['4'] = 0x04;
    shuntcocttab['5'] = 0x05;
    shuntcocttab['6'] = 0x06;
    shuntcocttab['7'] = 0x07;
    /* float tables */
    shuntcdecflttab['0'] = 0.0f;
    shuntcdecflttab['1'] = 1.0f;
    shuntcdecflttab['2'] = 2.0f;
    shuntcdecflttab['3'] = 3.0f;
    shuntcdecflttab['4'] = 4.0f;
    shuntcdecflttab['5'] = 5.0f;
    shuntcdecflttab['6'] = 6.0f;
    shuntcdecflttab['7'] = 7.0f;
    shuntcdecflttab['8'] = 8.0f;
    shuntcdecflttab['9'] = 9.0f;
    shuntchexflttab['0'] = 0.0f;
    shuntchexflttab['1'] = 1.0f;
    shuntchexflttab['2'] = 2.0f;
    shuntchexflttab['3'] = 3.0f;
    shuntchexflttab['4'] = 4.0f;
    shuntchexflttab['5'] = 5.0f;
    shuntchexflttab['6'] = 6.0f;
    shuntchexflttab['7'] = 7.0f;
    shuntchexflttab['8'] = 8.0f;
    shuntchexflttab['9'] = 9.0f;
    shuntchexflttab['a'] = 10.0f;
    shuntchexflttab['b'] = 11.0f;
    shuntchexflttab['c'] = 12.0f;
    shuntchexflttab['d'] = 13.0f;
    shuntchexflttab['e'] = 14.0f;
    shuntchexflttab['f'] = 15.0f;
    shuntchexflttab['A'] = 10.0f;
    shuntchexflttab['B'] = 11.0f;
    shuntchexflttab['C'] = 12.0f;
    shuntchexflttab['D'] = 13.0f;
    shuntchexflttab['E'] = 14.0f;
    shuntchexflttab['F'] = 15.0f;
    shuntcoctflttab['0'] = 0.0f;
    shuntcoctflttab['1'] = 1.0f;
    shuntcoctflttab['2'] = 2.0f;
    shuntcoctflttab['3'] = 3.0f;
    shuntcoctflttab['4'] = 4.0f;
    shuntcoctflttab['5'] = 5.0f;
    shuntcoctflttab['6'] = 6.0f;
    shuntcoctflttab['7'] = 7.0f;
    /* double tables */
    shuntcdecdbltab['0'] = 0.0;
    shuntcdecdbltab['1'] = 1.0;
    shuntcdecdbltab['2'] = 2.0;
    shuntcdecdbltab['3'] = 3.0;
    shuntcdecdbltab['4'] = 4.0;
    shuntcdecdbltab['5'] = 5.0;
    shuntcdecdbltab['6'] = 6.0;
    shuntcdecdbltab['7'] = 7.0;
    shuntcdecdbltab['8'] = 8.0;
    shuntcdecdbltab['9'] = 9.0;
    shuntchexdbltab['0'] = 0.0;
    shuntchexdbltab['1'] = 1.0;
    shuntchexdbltab['2'] = 2.0;
    shuntchexdbltab['3'] = 3.0;
    shuntchexdbltab['4'] = 4.0;
    shuntchexdbltab['5'] = 5.0;
    shuntchexdbltab['6'] = 6.0;
    shuntchexdbltab['7'] = 7.0;
    shuntchexdbltab['8'] = 8.0;
    shuntchexdbltab['9'] = 9.0;
    shuntchexdbltab['a'] = 10.0;
    shuntchexdbltab['b'] = 11.0;
    shuntchexdbltab['c'] = 12.0;
    shuntchexdbltab['d'] = 13.0;
    shuntchexdbltab['e'] = 14.0;
    shuntchexdbltab['f'] = 15.0;
    shuntchexdbltab['A'] = 10.0;
    shuntchexdbltab['B'] = 11.0;
    shuntchexdbltab['C'] = 12.0;
    shuntchexdbltab['D'] = 13.0;
    shuntchexdbltab['E'] = 14.0;
    shuntchexdbltab['F'] = 15.0;
    shuntcoctdbltab['0'] = 0.0;
    shuntcoctdbltab['1'] = 1.0;
    shuntcoctdbltab['2'] = 2.0;
    shuntcoctdbltab['3'] = 3.0;
    shuntcoctdbltab['4'] = 4.0;
    shuntcoctdbltab['5'] = 5.0;
    shuntcoctdbltab['6'] = 6.0;
    shuntcoctdbltab['7'] = 7.0;

    return;
}

void
shuntcinitop(long *chartab, long *prectab, long *nargtab)
{
    /* lookup table */
    chartab['~'] = '~';
    chartab['&'] = '&';
    chartab['|'] = '|';
    chartab['^'] = '^';
    chartab['<'] = '<';
    chartab['>'] = '>';
    chartab['+'] = '+';
    chartab['-'] = '-';
    chartab['*'] = '*';
    chartab['/'] = '/';
    chartab['%'] = '%';
    chartab['='] = '=';
    /* precedences */
    prectab[SHUNTCNOT] = SHUNTCRTOL | 8;
    prectab[SHUNTCINC] = 8;
    prectab[SHUNTCDEC] = 8;
    prectab[SHUNTCSHL] = 7;
    prectab[SHUNTCSHR] = 7;
    prectab[SHUNTCAND] = 6;
    prectab[SHUNTCXOR] = SHUNTCRTOL | 5;
    prectab[SHUNTCOR] = 4;
    prectab[SHUNTCMUL] = 3;
    prectab[SHUNTCDIV] = 3;
    prectab[SHUNTCMOD] = 3;
    prectab[SHUNTCADD] = SHUNTCRTOL | 2;
    prectab[SHUNTCSUB] = 2;
    prectab[SHUNTCASSIGN] = 1;
    /* # of arguments */
    nargtab[SHUNTCNOT] = 1;
    nargtab[SHUNTCINC] = 1;
    nargtab[SHUNTCDEC] = 1;
    nargtab[SHUNTCSHL] = 2;
    nargtab[SHUNTCSHR] = 2;
    nargtab[SHUNTCAND] = 2;
    nargtab[SHUNTCXOR] = 2;
    nargtab[SHUNTCOR] = 2;
    nargtab[SHUNTCMUL] = 2;
    nargtab[SHUNTCDIV] = 2;
    nargtab[SHUNTCMOD] = 2;
    nargtab[SHUNTCADD] = 2;
    nargtab[SHUNTCSUB] = 2;

    return;
}

void
shuntcinitoptab(void)
{
    shuntcinitop(shuntcopchartab, shuntcopprectab, shuntcopnargtab);
    /* evaluation functions */
    shuntcevaltab[SHUNTCNOT] = shuntcnot64;
    shuntcevaltab[SHUNTCINC] = shuntcinc64;
    shuntcevaltab[SHUNTCDEC] = shuntcdec64;
    shuntcevaltab[SHUNTCSHL] = shuntcshl64;
    shuntcevaltab[SHUNTCSHR] = shuntcshr64;
    shuntcevaltab[SHUNTCAND] = shuntcand64;
    shuntcevaltab[SHUNTCXOR] = shuntcxor64;
    shuntcevaltab[SHUNTCOR] = shuntcor64;
    shuntcevaltab[SHUNTCMUL] = shuntcmul64;
    shuntcevaltab[SHUNTCDIV] = shuntcdiv64;
    shuntcevaltab[SHUNTCMOD] = shuntcmod64;
    shuntcevaltab[SHUNTCADD] = shuntcadd64;
    shuntcevaltab[SHUNTCSUB] = shuntcsub64;
    shuntcevaltab[SHUNTCSAR] = shuntcsar64;

    return;
}

void
shuntcinitexpr(void)
{
    shuntcinitconvtab();
    shuntcinitoptab();
//    shuntcinitop();

    return;
}

