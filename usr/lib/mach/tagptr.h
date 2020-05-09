#ifndef __MACH_TAGPTR_H__
#define __MACH_TAGPTR_H__

/* tagged pointers for libmach */

#include <stdint.h>
#include <mach/bignum.h>

#if ((defined(__i386__) || defined(__i486__)                            \
      || defined(__i586__) || defined(__i686__))                        \
     && (!defined(__x86_64__) && !defined(__amd64__) && !defined(SSE)))

#include <mach/ia32/tagptr.h>

#elif defined(__x86_64__) || defined(__amd64__)

#include <mach/x86-64/tagptr.h>

#endif

#endif /* __MACH_TAGPTR_H__ */

