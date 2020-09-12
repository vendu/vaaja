#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <zero/cdefs.h>

void                   *memcpy(void *dest, const void *src, size_t len);
void                   *memmove(void * C_RESTRICT dest,
                                const void * C_RESTRICT src,
                                size_t n);

#endif /* __STRING_H__ */

