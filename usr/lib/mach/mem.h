#ifndef __MACH_MEM_H__
#define __MACH_MEM_H__

/* for systems without GNU C, we define these in e.g. <mach/ia32/asm.h> */
#if defined(__GNUC__)
/* stack frames */
#define m_atomread(a)      (*((typeof(a))(a)))
#define m_atomwrite(a, v)  (*((typeof(v) *)(a)) = (v))
#define m_syncread(a, res)                                              \
    do {                                                                \
        m_memwrbar();                                                   \
        (res) = m_atomread(a);                                          \
    } while (0)
#define m_syncwrite(a, val)                                             \
    do {                                                                \
        m_memwrbar();                                                   \
        m_atomwrite(a, val);                                            \
    } while (0)
#endif

#endif /* __MACH_MEM_H__ */

