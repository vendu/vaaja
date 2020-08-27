#ifndef __SYS_ZEN_BITBANG_H__
#define __SYS_ZEN_BITBANG_H__

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)

#define m_clz32(x)      ((x) ? __builtin_clz(x) : 32)
#define m_clz64(x)      ((x) ? __builtin_clzll(x) : 64)
#define m_ctz32(x)      ((x) ? __builtin_ctz(x) : 32)
#define m_ctz64(x)      ((x) ? __builtin_ctzll(x) : 64)
#define m_ham32(x)      __builtin_popcount(x)
#define m_ham64(x)      __builtin_popcountll(x)
#define bswap16(x)      __builtin_bswap16(x)
#define bswap32(x)      __builtin_bswap32(x)
#define bswap64(x)      __builtin_bswap64(x)

#else /* !(defined(__GNUC__ || defined(__clang__))) */

#define ZEN_NEED_CLZ    1
#define ZEN_NEED_CTZ    1
#define ZEN_NEED_HAM    1

#define bswap16(u)                                                      \
    ((((u) & 0xff00U) >> 8)                                             \
     | (((u) & 0x00ffU) << 8))
#define bswap32(u)                                                      \
    ((((u) & UINT32_C(0xff000000U)) >> 24)                              \
     | (((u) & UINT32_C(0x00ff0000U)) >> 8)                             \
     | (((u) & UINT32_C(0x0000ff00U)) << 8)                             \
     | (((u) & UINT32_C(0x000000ffU)) << 24))
#define bswap64(u)                                                      \
    ((((u) & UINT64_C(0xff00000000000000)) >> 56)                       \
     | (((u) & UINT64_C(0x00ff000000000000)) >> 40)                     \
     | (((u) & UINT64_C(0x0000ff0000000000)) >> 24)                     \
     | (((u) & UINT64_C(0x000000ff00000000)) >> 8)                      \
     | (((u) & UINT64_C(0x00000000ff000000)) << 8)                      \
     | (((u) & UINT64_C(0x0000000000ff0000)) << 24)                     \
     | (((u) & UINT64_C(0x000000000000ff00)) << 40)                     \
     | (((u) & UINT64_C(0x00000000000000ff)) << 56))

#endif

#endif /* __SYS_ZEN_BITBANG_H__ */

