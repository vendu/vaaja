#ifndef __MACH_ASM_H__
#define __MACH_ASM_H__

#include <mach/atomic.h>
//#include <mach/mem.h>
#if defined(__v0__)
#include <mach/v0/asm.h>
#elif defined(_WIN64)
#include <zero/msc/win64.h>
#elif defined(_WIN32)
#include <zero/msc/win32.h>
#elif (defined(__x86_64__) || defined(__amd64__))
#include <mach/x86/asm.h>
#include <mach/amd64/asm.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <mach/x86/asm.h>
#include <mach/ia32/asm.h>
#elif defined(__arm__)
#include <mach/arm/asm.h>
#elif defined(__ppc__)
#include <mach/ppc/asm.h>
#endif

#define m_trylkbit(p, ndx)      (!m_cmpsetbit(p, ndx))
#define m_unlkbit(p, ndx)       m_clrbit(p, ndx)
#define m_lkbit(p, ndx)                                                 \
    do {                                                                \
        long _bit = 1 << (ndx);                                         \
        long _res;                                                      \
                                                                        \
        do {                                                            \
            while ((uintptr_t)(p) & _bit) {                             \
                m_waitspin();                                           \
            }                                                           \
            _res = m_cmpsetbit((p), ndx);                               \
        } while (_res);                                                 \
    } while (0)

#endif /* __MACH_ASM_H__ */

