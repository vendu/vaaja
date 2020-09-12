#ifndef  __MACH_TYPES_H__
#define  __MACH_TYPES_H__

#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>

#if defined(__v0__)
#include <mach/v0/types.h>
#elif defined(_WIN64)
#include <zero/msc/win64.h>
#elif defined(_WIN32)
#include <zero/msc/win32.h>
#elif defined(_x86_64__) || defined(__amd64__)
#include <mach/x86/types.h>
#include <mach/amd64/types.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <mach/x86/types.h>
#include <mach/ia32/types.h>
#elif defined(__arm__)
#include <mach/arm/types.h>
#elif defined(__ppc__)
#include <mach/ppc/types.h>
#endif

#endif /* __MACH_TYPES_H__ */

