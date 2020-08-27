#ifndef __BITS_ENDIAN_H__
#define __BITS_ENDIAN_H__

#if defined(__ppc__)
#define __BYTE_ORDER __BIG_ENDIAN
#else
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#endif /* __BITS_ENDIAN_H__ */

