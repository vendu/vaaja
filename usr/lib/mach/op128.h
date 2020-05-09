#ifndef __MACH_OP128_H__
#define __MACH_OP128_H__

#if defined(__x86_64__) || defined(__amd64__)

#include <stdint.h>
#include <mach/param.h>

#if (defined(__clang__)                                                 \
     && ((__clang_major__ > 3)                                          \
         || (__clang_major__ == 3 && __clang_minor__ >= 1))             \
     && !defined(__SIZEOF_INT128__))
#define __SIZEOF_INT128__   16
#endif
#if defined(__SIZEOF_INT128__)
typedef __int128_t          op128_t;
#define OP128_ZERO          ((op128_t)0)
#define opdwload(ptr)       (*(ptr))
#define opdwstore(val, ptr) (*(ptr) = (val))
#define opdwcpy(src, dest)  (*(dest) = (src))
#elif defined(__SSE2__) || defined(__AVX__)
#include <emmintrin.h>
typedef __m128i             op128_t;
#define OP128_ZERO          _mm_setzero_si128()
#define opdwload(ptr)       _mm_load_si128(ptr)
#define opdwstore(val, ptr) _mm_store_si128(ptr, val)
#define opdwcpy(src, dest)  (*(__m128i *)(dest) = _mm_store_si128(src))
#else /* !__SSE__ && !__AVX__ */
#define OP128_ZERO { UINT64_C(0), INT64_C(0) }
typedef struct {
    uint64_t lo;
    int64_t  hi;
} op128_t;
#endif

#define opldzero128(val)    ((val) = OP128_ZERO)
#define opstzero128(ptr)    (*(op128_t *)(ptr) = OP128_ZERO)

#endif /* __x86_64__ || __amd64__ */

#endif /* __MACH_OP128_H__ */

