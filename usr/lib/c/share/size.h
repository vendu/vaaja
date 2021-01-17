#ifndef SHARE_SIZE_H
#define SHARE_SIZE_H

#include <mach/param.h>

#if (MACH_PTR_SIZE == 8)
typedef unsigned long long      size_t;
#elif (MACH_PTR_SIZE == 4)
typedef uint32_t                size_t;
#endif
#define _size_t_defined         1

#endif /* SHARE_SIZE_H */

