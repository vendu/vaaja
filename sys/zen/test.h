#ifndef __SYS_ZEN_TEST_H__
#define __SYS_ZEN_TEST_H__

#include <stdlib.h>

#define kmalloc(sz)             malloc(sz)
#define kfree(ptr)              free(ptr)
#define kmemalign(sz, aln)      aligned_alloc(aln, sz)

#endif /* __SYS_ZEN_TEST_H__ */

