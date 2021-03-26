#ifndef __KERN_UNIT_X86_ASM_H__
#define __KERN_UNIT_X86_ASM_H__

#include <stdint.h>

#define k_halt()    __asm__ __volatile__ ("hlt\n");
#define k_introff() __asm__ __volatile__ ("cli\n" : : : "memory")
#define k_intron()  __asm__ __volatile__ ("sti\n" : : : "memory")
#define k_waitint() __asm__ __volatile__ ("sti\nhlt\n" : : : "memory")

static __inline__ uint64_t
k_readmsr(uint32_t adr)
{
    uint32_t eax;
    uint32_t edx;
    uint64_t retval;

    __asm__ __volatile__ ("rdmsr\n"
                          : "=a" (eax), "=d" (edx)
                          : "r" (adr));
    retval = ((uint64_t)edx << 32) | eax;

    return retval;
}

static __inline__ void
k_writemsr(uint32_t adr, uint64_t val)
{
    __asm__ __volatile__ ("wrmsr\n" : : "c" (adr), "A" (val));
}

#if ((defined(__i386__) || defined(__i486__)                            \
      || defined(__i568__) || defined(__i686__))                        \
     && (defined(__x86_64__) || defined(__amd64__)))
#include <kern/unit/ia32/asm.h>
#else
#include <kern/unit/x86-64/asm.h>
#endif

#endif /* __KERN_UNIT_X86_ASM_H__ */

