#include <stdint.h>

double
k_sin(double x)
{
    double retval;

    __asm__ __volatile__ ("fldl %0\n" : : "m" (x));
    __asm__ __volatile__ ("fsin\n");
    __asm__ __volatile__ ("fstpl %0\n"
                          "fwait\n"
                          : "=m" (retval));

    return retval;
}

double
k_ceil(double d)
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

