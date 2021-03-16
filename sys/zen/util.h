#ifndef SYS_ZEN_UTIL_H
#define SYS_ZEN_UTIL_H

#include <zen/fastudiv.h>
extern struct divu16            k_fastu16divu16tab[SCHED_ULE_DIVU16TAB_SIZE] C_ALIGNED(MACH_PAGE_SIZE);


#define EOF                     (-1)

#define zensrand(seed)          (srandmt32(seed))
#define zenurand()              (randmt32())
#define _roundup16b(a, b)                                               \
    ((fastu16divu16((a) + (b) - 1, (b), k_fastu16divu16tab)) * (b))

#define kdebug(s, p)                                                    \
    do {                                                                \
        kprintf("%s: %p\n", s, p);                                      \
        do {                                                            \
            m_waitspin();                                               \
        } while (1);                                                    \
    } while (0);

void                            kbzero(void *adr, unsigned long len);
void                            kmemset(void *adr, int byte, unsigned long len);
#define kbcopy(src, dest, len)  kmemcpy(dest, src, len);
void                            kbcopy(void *dest, const void *src, unsigned long len);
int                             kmemcmp(const void *ptr1, const void *ptr2, unsigned long nb);
int                             kstrcmp(const char *str1, const char *str2);
long                            kstrncpy(char *dest, char *src, long len);
void                            kprintf(const char *fmt, ...);
void                    i       kperror(const char *str);
void                            kpanic(const char *str, int sig);

#endif /* SYS_ZEN_UTIL_H */

