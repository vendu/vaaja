#ifndef __UNIT_X86_64_TRAP_H__
#define __UNIT_X86_64_TRAP_H__

#include <kern/unit/x86/trap.h>

#define trapsetintgate(p64, func, ring)                                 \
    do {                                                                \
        uint64_t mask = 0xffffffff;                                     \
                                                                        \
        (p64)[0] = trapmkdesc(TEXTSEL, (func) & mask, ring);            \
        (p64)[1] = ((uint64_t)(func) >> 32) & mask;                     \
    } while (0)

#endif /* __UNIT_X86_64_TRAP_H__ */

