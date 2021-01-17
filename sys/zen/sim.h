#ifndef SYS_ZEN_SIM_H
#define SYS_ZEN_SIM_H

/* header file to replace certain routines with libc-ones */

#include <stdlib.h>

#define kmalloc(sz)             malloc(sz)
#define kfree(ptr)              free(ptr)
#define kmemalign(sz, aln)      aligned_alloc(aln, sz)

#endif /* SYS_ZEN_SIM_H */

