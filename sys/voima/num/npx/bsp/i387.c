/*
 * REFERENCES: https://www.felixcloutier.com/x86/index.html
 */

/* i387 math library for voima virtual machine */

/* TODO: FCMOVB, FCMOVBE, FCMOVE, FCMOVNB, FCMOVNBE, FCMOVNE, FCMOVNU, FCMOVU */
/* TODO: FCOMI, FCOMIP, FUCOMI, FUCOMIP */

#include <stdint.h>
#if (TEST_NUM_X87)
#include <stdlib.h>
#include <stdio.h>
#endif

/* no operation */
void
m_fnop(double x)
{
    __asm__ __volatile__ ("fnop\n");

    return;
}

/* NPX CONTROL ROUTINES */

/* initialize floating point processor, no wait for exceptions */
void
m_fninit(void)
{
    __asm__ __volatile__ ("fninit\n"
                          "fwait\n");

    return;
}

/* load control word */
void
m_fldcw(uint16_t u16)
{
    __asm__ __volatile__ ("fldcw %0\n"
                          "fwait\n"
                          :
                          : "m" (u16));

    return;
}

/* load environment state */
void
m_fldenv(fenv_t *env)
{
    __asm__ __volatile__ ("fldenv %0\n"
                          "fwait\n"
                          : "m" (*(env))
                          :);

    return;
}

/* wait while FPU is executing */
void
m_fwait(double x)
{
    __asm__ __volatile__ ("fwait\n");

    return;
}

/* clear exceptions, check pending exceptions */
void
m_fclex(void)
{
    __asm__ __volatile__ ("fclex\n");

    return;
}

/* clear exceptions, no wait */
void
m_fnclex(void)
{
    __asm__ __volatile__ ("fnclex\n");

    return;
}

/* read control word, check pending exceptions */
uint16_t
m_fstcw(void)
{
    uint16_t                    u16;

    __asm__ __volatile__ ("fstcw %0\n"
                          "fwait\n"
                          : "m" (u16));

    return u16;
}

/* read control word, no wait */
uint16_t
m_fnstcw(void)
{
    uint16_t                    u16;

    __asm__ __volatile__ ("fnstcw %0\n"
                          "fwait\n"
                          : "m" (u16));
    return u16;
}

/* read status word, check pending exceptions */
uint16_t
m_fstsw(void)
{
    uint16_t                    u16;

    __asm__ __volatile__ ("fstsw %0\n"
                          "fwait\n"
                          : "rm" (u16));

    return u16;
}

/* read status word, no wait */
uint16_t
m_fnstsw(void)
{
    uint16_t                    u16;

    __asm__ __volatile__ ("fnstsw %0\n"
                          "fwait\n"
                          : "rm" (u16));

    return u16;
}

/* store FPU environment, check for pending exceptions */
void
m_fstenv(fenv_t *env)
{
    __asm__ __volatile__ ("fstenv %0\n"
                          "fwait\n"
                          : "=m" (*(env)));

    return;
}

/* store FPU environment, no wait */
void
m_fnstenv(fenv_t *env)
{
    __asm__ __volatile__ ("fnstenv %0\n"
                          "fwait\n"
                          : "=m" (*(env)));

    return;
}

/* CONTEXT OPERATIONS */

/* check pending exceptions, save FPU state, reinitialise the FPU */
void
m_fsave(fenvi387_t *ctx)
{
    __asm__ __volatile__ ("fsave %0\n"
                          : "=m" (*(ctx)));

    return;
}

/* save FPU state without checking exceptions, reinitialise the FPU */
void
m_fnsave(fenvi387_t *ctx)
{
    __asm__ __volatile__ ("fnsave %0\n"
                          : "=m" (*(ctx)));

    return;
}

/* restore saved state */
void
m_frstor(fenvi387_t *ctx)
{
    __asm__ __volatile__ ("frstor %0\n"
                          : "=m" (*(ctx)));

    return;
}

/* decrement floating point stack pointer */
void
m_fdecstp(void)
{
    __asm__ __volatile__ ("fdecstp\n");

    return;
}

/* increment floating poiunt stack pointer */
void
m_fincstp(void)
{
    __asm__ __volatile__ ("fincstp\n");

    return;
}

/* free register st(1) */
double
m_ffree1(void)
{
    __asm__ __volatile__ ("ffree st(1)\n");

    return;
}

/* free register st(2) */
double
m_ffree2(void)
{
    __asm__ __volatile__ ("ffree st(2)\n");

    return;
}

/* free register st(3) */
double
m_ffree3(void)
{
    __asm__ __volatile__ ("ffree st(3)\n");

    return;
}

/* free register st(4) */
double
m_ffree4(void)
{
    __asm__ __volatile__ ("ffree st(4)\n");

    return;
}

/* free register  st(5) */
double
m_ffree5(void)
{
    __asm__ __volatile__ ("ffree st(5)\n");

    return;
}

/* free register  st(6) */
double
m_ffree6(void)
{
    __asm__ __volatile__ ("ffree st(6)\n");

    return;
}

/* free register  st(7) */
double
m_ffree7(void)
{
    __asm__ __volatile__ ("ffree st(7)\n");

    return;
}

/* free register  st(8) */
double
m_ffree8(void)
{
    __asm__ __volatile__ ("ffree st(8)\n");

    return;
}

/* absolute value */
double
m_fabs(double x)
{
    double              ret;

    __asm__ __volatile__ ("fldl %1\n"
                          "fabs\n"
                          "fstpl %0\n"
                          "fwait\n"
                          : "=rm" (ret)
                          : "rm" (x));

    return ret;
}

/* arithmetic floating point operations */

/* addition */
double
m_fadd(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fadd\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* addition and pop */
double
m_faddp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("faddp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point subtraction */
double
m_fsub(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsub\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point subtraction and pop */
double
m_fsubp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsubp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse floating point subtraction */
double
m_fsubr(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsubr\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse floating point subtraction and pop */
double
m_fsubrp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsubrp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point multiplication */
double
m_fmul(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fmul\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point multiply and pop */
double
m_fmulp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fmulp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* division */
double
m_fdiv(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fdiv\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* division and pop */
double
m_fdivp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fdivp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse divisin */
double
m_fdivr(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fdivr\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse division */
double
m_divrp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fdivrp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* partial remainder */
double
m_fprem1(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fprem1\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* arithmetic operations with integer operands */

/* add integer */
double
m_fiadd(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fiadd\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* integer subtraction */
double
m_fisub(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fisub\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* integer multiplication */
double
m_fimul(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fimul\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* integer division */
double
m_fidiv(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fidiv\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse integer division */
double
m_fidivr(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fidivr\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse integer division and pop */
double
m_fidivrp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fidivrp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* reverse integer subtraction */
double
m_fisubr(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fisubr\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare */
double
m_fcom(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fcom\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare and pop */
double
m_fcomp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fcomp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare and pop twice */
double
m_fcompp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fcompp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare */
double
m_fucom(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fucom\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare and pop */
double
m_fucomp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fucomp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* compare and pop twice */
double
m_fucompp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fucompp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* integer compare */
double
m_ficom(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ficom\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* integer compare and pop */
double
m_ficomp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ficomp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* test for zero */
double
m_ftst(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ftst\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* store integer */
double
m_fild(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fild\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* store integer */
double
m_fist(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fist\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* store integer and pop */
double
m_fistp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fistp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point load */
double
m_fld(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fld\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* floating point store */
double
m_fst(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fst\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load 1.0 onto stack */
double
m_fld1(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fld1\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load log2(e) onto stack */
double
m_fldl2e(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fldl2e\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load log2(10) onto stack */
double
m_fldl2t(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fldl2t\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load log10(2) onto stack */
double
m_fldlg2(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fldlg2\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load ln(2) onto stack */
double
m_ldln2(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ldln2\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load pi onto stack */
double
m_fldpi(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fldpi\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load 0.0 onto stack */
double
m_fldz(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fldz\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* load BCD */
double
m_fbld(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fbld\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* store BCD and pop */
double
m_fbstp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fbstp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* round to integer */
double
m_frndint(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("frndint\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* scale by factor of 2 */
double
m_scale(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("scale\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* square root */
double
m_fsqrt(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsqrt\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* 2^x - 1 */
double
m_f2xm1(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("f2xm1\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* store and pop */
double
m_fstp(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fstp\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* y * log2(x) */
double
m_fyl2x(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fyl2x\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* y * log2(x + 1) */
double
m_fyl2xp1(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fyl2xp1\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_sin(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsin\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_cos(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fcos\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_tan(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ftan\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_asin(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fasin\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_acos(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("facos\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_atan(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fatan\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_sinh(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsinh\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_cosh(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fcosh\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

double
m_tanh(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("ftanh\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* partial tangent */
double
m_ptan(double x)
{
    double ret;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fptan\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (ret));

    return ret;
}

/* partial arctangent */
double
m_fpatan(double x)
{
    double ret;

    __asm__ __volatile__ ("ffpatan %0\n"
                          : "=t" (ret)
                          : "0" (inp));

    return ret;
}

double
m_sincos(double x)
{
    double

    __asm__ __volatile__ ("fsincos"
                          : "=t" (cos), "=u" (sin)
                          : "0" (inp));

    return ret;
}

double
m_ceil(double d)
{
    double   ret;
    uint32_t fcw;
    uint32_t tmp;

    __asm__ __volatile__ ("fstcw %0\n"
                          "movw %0, %%dx\n"
                          "orw $0x0800, %%dx\n"
                          "andw $0xfbff, %%dx\n"
                          "movw %%dx, %1\n"
                          "fldcw %1\n"
                          "fldl %3\n"
                          "frndint\n"
                          "fstpl %2\n"
                          "fldcw %0\n"
                          "fwait\n"
                          : "=m" (fcw), "=m" (tmp), "=m" (ret)
                          : "m" (d)
                          : "edx");

    return ret;
}

#if (TEST_NUM_X87)

int
main(int argc, char *argv[])
{
    double  dbl;

    dbl = m_sin(3.0);
    if (dbl != sin(3.0)) {
        abort();
    }

    exit(0);
}

#endif
