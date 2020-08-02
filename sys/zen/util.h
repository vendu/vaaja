#ifndef __SYS_ZEN_UTIL_H__
#define __SYS_ZEN_UTIL_H__

#include <stdint.h>
#include <mach/asm.h>
#include <sys/zen/signal.h>

#define _roundup16b(a, b)                                               \
    ((fastu16divu16((a) + (b) - 1, (b), k_fastu16divu16tab)) * (b))

#define kdebug(s, p)                                                    \
    do {                                                                \
        kprintf("%s: %p\n", s, p);                                      \
        do {                                                            \
            m_waitspin();                                               \
        } while (1);                                                    \
    } while (0);

void kbzero(void *adr, unsigned long len);
void kmemset(void *adr, int byte, unsigned long len);
void kbcopy(void *dest, void *src, unsigned long len);
int  kmemcmp(const void *ptr1, const void *ptr2, unsigned long nb);
int  kstrcmp(const char *str1, const char *str2);
long kstrncpy(char *dest, char *src, long len);
void kprintf(const char *fmt, ...);
void kpanic(const char *str, int sig);

#endif /* __SYS_ZEN_UTIL_H__ */

