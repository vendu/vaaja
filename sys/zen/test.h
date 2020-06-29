#ifndef __ZEN_TEST_H__
#define __ZEN_TEST_H__

#include <stdlib.h>

#define kmalloc(sz)             malloc(sz)
#define kfree(ptr)              free(ptr)
#define kmemalign(sz, aln)      aligned_alloc(aln, sz)

#endif /* __ZEN_TEST_H__ */

