#ifndef __MACH_ARM_PROF_H__
#define __MACH_ARM_PROF_H__

/* TODO: fix this to work and find out which versions of ARM it may work with */

#define PROFTICK(id)                                                    \
    unsigned int __tv##id[2]
#define profclrtick(id)                                                 \
    __tv##id[0] = __tv##id[1] = 0
#define profstarttick(id)			                        \
    /* disable cycle counter */                                         \
    __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 2\n" : : "r" (1 << 31)); \
    /* initialize cycle counter */                                      \
    __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 0\n" : : "r" (5));  \
    /* enable cycle counter */                                          \
    __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 1\n" : : "r" (1 << 31)); \
    __tv##id[0] = _getccnt()
#define profstoptick(id)                                                \
    __tv##id[1] = _getccnt();                                           \
    /* disable cycle counter */
    __asm__ __volatile__ ("mcr p15, 0, %0, c9, c12, 2\n" : : "r" (1 << 31))

#define proftickdiff(id)                                                \
    (__tv##id[1] - __tv##id[0])

static __inline__
unsigned int
_getccnt(void)
{
    unsigned int val;

    __asm__ __volatile__ ("mrc p15, 0, %0, c9, c13, 0\n" : "=r" (val));

    return val;
}
#endif /* __MACH_ARM_PROF_H__ */

