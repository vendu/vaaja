#ifndef SYS_ZEN_INTERNAL_MACH_IA32_TYPES_H
#define SYS_ZEN_INTERNAL_MACH_IA32_TYPES_H

typedef int32_t                 m_atomic_t;

#define tf_pc                   eip
#define tf_ucs                  ucs
#define tf_msw                  eflags
#define tf_usp                  uesp
#define tf_ss                   uss
struct m_trapframe {
    int32_t                     eip;
    int16_t                     ucs, _pad1;
    int32_t                     eflags;
    int32_t                     uesp;
    int16_t                     uss, _pad2;
};

/* in the order for the PUSHA- and POPA-instructions */
struct m_genregs {
    int32_t                     edi;
    int32_t                     esi;
    int32_t                     ebp;
    int32_t                     esp;
    int32_t                     ebx;
    int32_t                     edx;
    int32_t                     ecx;
    int32_t                     eax;
};

#endif /* SYS_ZEN_INTERNAL_MACH_IA32_TYPES_H */

