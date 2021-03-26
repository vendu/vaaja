#include <stddef.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <mach/asm.h>

NOINLINE void *
m_getpc(void)
{
    void *ptr = NULL;

    ptr = m_getretadr();

    return ptr;
}

INLINE void *
m_getpc2(void)
{
    void *ptr;

    __asm__ __volatile__ ("leaq (%%rip), %0\n"
                          : "=r" (ptr));

    return ptr;
}

INLINE void *
m_getpc3(void)
{
    void *ptr;

    __asm__ __volatile__ ("movq $., %0\n"
                          : "=r" (ptr));

    return ptr;
}
