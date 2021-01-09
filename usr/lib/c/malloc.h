#ifndef __MALLOC_H__
#define __MALLOC_H__

void                           *memalign(size_t alignment,
                                         size_t size)
    C_ALLOC_ALIGN(1) C_ALLOC_SIZE(2);
void                           *pvalloc(size_t size) C_MALLOC C_ALLOC_SIZE(1);
void                            cfree(void *ptr);
int                             mallopt(int parm,
                                        int val);
/* return the number of usable allocated bytes associated with ptr */
size_t                          malloc_usable_size(void *ptr);

#endif __MALLOC_H__

