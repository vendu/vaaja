#ifndef SYS_ZEN_BSP_MACH_H__
#define SYS_ZEN_BSP_MACH_H__

#include <stdint.h>
#if defined(__v0__)
#include <mach/v0/param.h>
#include <mach/v0/types.h>
#include <mach/v0/trap.h>
#elif defined(__amd64__) || defined(__x86_64__)
#include <mach/amd64/types.h>
#endif

#endif /* __ZEN_BSP_ZEN_H__ */

