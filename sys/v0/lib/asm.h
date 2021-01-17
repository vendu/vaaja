#ifndef DECK_LIB_ASM_H
#define DECK_LIB_ASM_H

#include <deck/lib/cdefs.h>
#include <stdint.h>

#if (defined(__amd64__)                                                 \
     || defined(__x86_64__))
//#include <deck/lib/asm-amd64.h>
#elif (defined(__i386__)                                                \
       || defined(__i486__)                                             \
       || defined(__i586__)                                             \
       || defined(__i686__))
#include <deck/lib/asm-ia32.h>
#endif

#endif /* DECK_LIB_ASM_H */

