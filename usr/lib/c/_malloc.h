#ifndef ___MALLOC_H__
#define ___MALLOC_H__

#include <mach/param.h>

#define MALLOC_MIN_BLK      MACH_CL_SIZE
#define MALLOC_BLK_SLOTS    32
#define MALLOC_BLK_SLAB     (2 * MACH_PAGE_SIZE)
#define MALLOC_MAX_BLK      (MALLOC_BLK_SLOTS * MALLOC_MIN_SIZE)
#define MALLOC_MIN_RUN      MACH_PAGE_SIZE
#define MALLOC_RUN_SLOTS    32
#define MALLOC_RUN_SLAB     (2 * MALLOC_MAX_RUN)
#define MALLOC_MAX_RUN      (MALLOC_RUN_SLOTS * MALLOC_MIN_RUN)

#define mallocblksize(i)    ((i) * MALLOC_MIN_BLK)
#define mallocrunsize(i)    ((i) * MALLOC_MIN_RUN)

#endif /* ___MALLOC_H__ */

