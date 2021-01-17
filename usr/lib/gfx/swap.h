#ifndef __GFX_SWAP_H__
#define __GFX_SWAP_H__

#include <stdint.h>
#include <endian.h>
#include <mach/param.h>
#include <gfx/rgb.h>

#if (__BYTE_ORDER == __BIG_ENDIAN)
#define gfxswap16(u)            bswap_16(u)
#define gfxswap32(u)            bswap_32(u)
#define gfxswap64(u)            bswap_64(u)
#else
#define gfxswap16(u)            (u)
#define gfxswap32(u)            (u)
#define gfxswap64(u)            (u)
#endif

extern uint8_t                  __bitswaptable[256];

#if defined(__HAVE_BYTESWAP_H__)

#include <byteswap.h>

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

#if defined(GFX_64_BIT_PROTOCOL)
#   define gfxswapproto(u)      gfxswap64(u)
#elif defined(GFX_32_BIT_PROTOCOL)
#   define gfxswapproto(u)      gfxswap32(u)
#endif

#define bswapargb(u) gfxswap32(u)

#define bswaprgb(p)                                                     \
    do {                                                                \
        uint8_t *__ptr1;                                                \
        uint8_t *__ptr3;                                                \
        uint8_t __val1;                                                 \
        uint8_t __val3;                                                 \
                                                                        \
        __ptr1 = (p);                                                   \
        __ptr3 = __ptr1 + 2;                                            \
        __val1 = *__ptr1;                                               \
        __val3 = *__ptr3;                                               \
        *__ptr1 = __val3;                                               \
        *__ptr3 = __val1;                                               \
    } while (FALSE);

/*
 * Bit-swapping. This is used for swapping bitmaps.
 */

#define bitswap_8(u) (__bitswaptable[(u)])
#define bitswap_16(u)                                                   \
    (((bitswap_8((u) & 0xff00U) >> 8))                                  \
     | ((bitswap_8((u) & 0x00ffU) << 8)))
#define bitswap_32(u)                                                   \
    (((bitswap_8((u) & 0xff000000U) >> 24))                             \
     | ((bitswap_8((u) & 0x00ff0000U) >> 8))                            \
     | ((bitswap_8((u) & 0x0000ff00U) << 24))                           \
     | ((bitswap_8((u) & 0x000000ffU) << 8)))
#define bitswap_64(u)                                                   \
    (((bitswap_8((u) & UINT64_C(0xff00000000000000) >> 56))             \
      | ((bitswap_8((u) & UINT64_C(0x00ff000000000000)) >> 40))         \
      | ((bitswap_8((u) & UINT64_C(0x0000ff0000000000)) >> 24))         \
      | ((bitswap_8((u) & UINT64_C(0x000000ff00000000)) >> 8))          \
      | ((bitswap_8((u) & UINT64_C(0x00000000ff000000)) << 8))          \
      | ((bitswap_8((u) & UINT64_C(0x0000000000ff0000)) << 24))         \
      | ((bitswap_8((u) & UINT64_C(0x000000000000ff00)) << 32))         \
      | ((bitswap_8((u) & UINT64_C(0x00000000000000ff)) << 56)))

#endif /* __GFX_SWAP_H__ */

