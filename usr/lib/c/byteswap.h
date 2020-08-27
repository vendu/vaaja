#ifndef __BYTESWAP_H__
#define __BYTESWAP_H__

#if defined(__GNUC__) || defined(__clang__)

#define bswap_16(x)     __builtin_bswap16(x)
#define bswap_32(x)     __builtin_bswap32(x)
#define bswap_64(x)     __builtin_bswap64(x)

#else

#define bswap_16(u)                                                     \
    ((((u) & 0xff00U) >> 8)                                             \
     | (((u) & 0x00ffU) << 8))
#define bswap_32(u)                                                     \
    ((((u) & 0xff000000U) >> 24)                                        \
     | (((u) & 0x00ff0000U) >> 8)                                       \
     | (((u) & 0x0000ff00U) << 8)                                       \
     | (((u) & 0x000000ffU) << 24))
#define bswap_64(u)                                                     \
    ((((u) & UINT64_C(0xff00000000000000)) >> 56)                       \
     | (((u) & UINT64_C(0x00ff000000000000)) >> 40)                     \
     | (((u) & UINT64_C(0x0000ff0000000000)) >> 24)                     \
     | (((u) & UINT64_C(0x000000ff00000000)) >> 8)                      \
     | (((u) & UINT64_C(0x00000000ff000000)) << 8)                      \
     | (((u) & UINT64_C(0x0000000000ff0000)) << 24)                     \
     | (((u) & UINT64_C(0x000000000000ff00)) << 40)                     \
     | (((u) & UINT64_C(0x00000000000000ff)) << 56))

#endif

#endif /* __BYTESWAP_H__ */

