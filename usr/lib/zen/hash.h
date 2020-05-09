#ifndef __ZEN_HASH_H__
#define __ZEN_HASH_H__

#define ZEN_INLINE_HASH 1

#include <stdint.h>
#include <zero/cdefs.h>

#if !defined(ZEN_INLINE_HASH)

CONST uint32_t  tmhash32(uint32_t u);
CONST uint32_t  tmunhash32(uint32_t u);
CONST uint32_t  tmhash64(uint32_t u);
CONST uint32_t  tmunhash64(uint32_t u);

#endif

#endif /* __ZEN_HASH_H__ */

