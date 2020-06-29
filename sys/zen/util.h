#ifndef __KERN_UTIL_H__
#define __KERN_UTIL_H__

#include <stdint.h>

void kbzero(void *adr, unsigned long len);
void kmemset(void *adr, int byte, unsigned long len);
void kbcopy(void *dest, void *src, unsigned long len);
int  kmemcmp(const void *ptr1, const void *ptr2, unsigned long nb);
int  kstrcmp(const char *str1, const char *str2);
long kstrncpy(char *dest, char *src, long len);
void kprintf(char *fmt, ...);
void kpanic(void);

#endif /* __KERN_UTIL_H__ */

