#ifndef __MACH_X86_SATMATH_H__
#define __MACH_X86_SATMATH_H__

static __inline__ satu8_t
_saddu8(satu8_t u1, satu8_t u2)
{
    satu8_t res;

    __asm__ __volatile__ ("addb %0, %2\n"
                          "sbbb %1, %1\n"
                          "orb %1, %2\n"
                          : "+r" (u2), "+a" (res)
                          : "r" (u1)
                          : "eax");

    return res;
}

#if !defined(CMOVNC)
static __inline__ satu8_t
_ssubu8(satu8_t u1, satu8_t u2)
{
    satu8_t res;

    __asm__ __volatile__ ("subb %2, %1\n"
                          "cmc\n"
                          "sbbb %0. %0\n"
                          "andb %1, %0\n"
                          : "+a" (res), "+D" (u2)
                          : "S" (u1)
                          : "eax");

    return res;
}
#else
static __inline__ satu8_t
_ssubu8(satu8_t u1, satu8_t u2)
{
    satu8_t res;

    __asm__ __volatile__ ("xorb %0, %0\n"
                          "subb %2. %1\n"
                          "cmovncb %1, %0\n"
                          : "+a" (res), "+D" (u2)
                          : "S" (u1)
                          : "eax", "edi");

    return res;
}
#endif

static __inline__ satu8_t
_smulu8(satu8_t u1, satu8_t u2)
{
    satu8_t res;

    __asm__ __volatile__ ("movb %1, %0\n"
                          "mulb %2\n"
                          "sbbb %1, %1\n"
                          "orb %1, %0\n"
                          : "+r" (res), "+D" (u2)
                          : "S" (u1)
                          : "eax", "edi");

    return res;
}

static __inline__ satu8_t
_sdivu8(satu8_t u1, satu8_t u2)
{
    satu8_t res;

    __asm__ __volatile__ ("movb %2, %0\n"
                          "divb %1\n"
                          : "+a" (res)
                          : "S" (u1), "D" (u2)
                          : "eax");

    return res;
}

#endif /* __MACH_X86_SATMATH_H__ */

