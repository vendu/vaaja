#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include <features.h>
#include <bits/endian.h>

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321

#if defined(_BSD_SOURCE)
#define LITTLE_ENDIAN   __LITTLE_ENDIAN
#define BIG_ENDIAN      __BIG_ENDIAN
#define BYTE_ORDER      __BYTE_ORDER
#endif

#endif /* __ENDIAN_H__ */

