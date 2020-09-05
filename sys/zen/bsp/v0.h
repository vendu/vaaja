#ifndef __ZEN_BSP_V0_H__
#define __ZEN_BSP_V0_H__

#include <stdint.h>
#if defined(__v0__)
#include <sys/zen/bsp/v0/param.h>
#include <sys/zen/bsp/v0/types.h>
#elif defined(__amd64__) || defined(__x86_64__)
#include <sys/zen/bsp/amd64/types.h>
#endif

#endif /* __ZEN_BSP_ZEN_H__ */

