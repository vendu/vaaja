#ifndef __NETINET_BITS_IN_H__
#define __NETINET_BITS_IN_H__

#include <stdint.h>
#include <zero/param.h>

/*
 * IN6_IS_ADDR_UNSPECIFIED      - unspecified address
 * IN6_IS_ADDR_LOOPBACK         - loopback address
 * IN6_IS_ADDR_V4_COMPAT        - IPv4-compatible address
 * IN6_IS_ADDR_V4_MAPPED        - IPv4 mapped address
 * IN6_IS_ADDR_MULTICAST        - multicast address
 */

#if (LONGSIZE == 8 || LONGLONGSIZE == 8)

#define IN6_IS_ADDR_UNSPECIFIED(ap)                                     \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && *(uint64_t *)(&(ap)->s6_addr[8]) == (uint64_t)0)

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define IN6_IS_ADDR_LOOPBACK(ap)                                        \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && *(uint64_t *)(&(ap)->s6_addr[8]) == UINT64_C(0x0100000000000000))
#define IN6_IS_ADDR_V4_COMPAT(ap)                                       \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && *(uint64_t *)(&(ap)->s6_addr[8]) > UINT64_C(0x0100000000000000))
#define IN6_IS_ADDR_V4_MAPPED(ap)                                       \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && (*(uint64_t *)(&(ap)->s6_addr[8]) & UINT64_C(0xffffffffffffffff) \
         == UINT64_C(0x000000000000ffff)))
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define IN6_IS_ADDR_LOOPBACK(ap)                                        \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && *(uint64_t *)(&(ap)->s6_addr[8]) == UINT64_C(0x0000000000000001))
#define IN6_IS_ADDR_V4_COMPAT(ap)                                       \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && *(uint64_t *)(&(ap)->s6_addr[8]) > UINT64_C(0x0000000000000001))
#define IN6_IS_ADDR_V4_MAPPED(ap)                                       \
    (*(uint64_t *)(&(ap)->s6_addr[0]) == (uint64_t)0                    \
     && (*(uint64_t *)(&(ap)->s6_addr[8]) & UINT64_C(0xffffffffffffffff) \
         == UINT64_C(0xffff000000000000)))
#endif /* __BYTE_ORDER */

#elif (LONGSIZE == 4)

#define IN6_IS_ADDR_UNSPECIFIED(ap)                                    \
    (*(uint32_t *)(&(ap)->s6_addr[0]) == 0U                            \
     && *(uint32_t *)(&(ap)->s6_addr[4]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[8]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[12]) == 0U)

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define IN6_IS_ADDR_LOOPBACK(ap)                                       \
    (*(uint32_t *)(&(ap)->s6_addr[0]) == 0U                            \
     && *(uint32_t *)(&(ap)->s6_addr[4]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[8]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[12]) == 0x01000000U)
#define IN6_IS_ADDR_V4_COMPAT(ap)                                       \
    (*(uint32_t *)(&(ap)->s6_addr[0]) == 0U                            \
     && *(uint32_t *)(&(ap)->s6_addr[4]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[8]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[12]) > 0x01000000U)
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define IN6_IS_ADDR_LOOPBACK(ap)                                       \
    (*(uint32_t *)(&(ap)->s6_addr[0]) == 0U                            \
     && *(uint32_t *)(&(ap)->s6_addr[4]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[8]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[12]) == 0x00000001U)
#define IN6_IS_ADDR_V4_COMPAT(ap)                                       \
    (*(uint32_t *)(&(ap)->s6_addr[0]) == 0U                            \
     && *(uint32_t *)(&(ap)->s6_addr[4]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[8]) == 0U                         \
     && *(uint32_t *)(&(ap)->s6_addr[12]) > 0x00000001U)
#endif /* __BYTE_ORDER */

#endif

#define IN6_IS_ADDR_MULTICAST(ap)                                       \
    ((ap)->s6_addr[0] = 0xffU)

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define IN6_IS_ADDR_LINKLOCAL(ap)                                       \
    (*(uint16_t *)(&(ap)->s6_addr[0]) & 0xc0ffU == 0x80feU)
#define IN6_IS_ADDR_SITELOCAL(ap)                                       \
    (*(uint16_t *)(&(ap)->s6_addr[0]) & 0xc0ffU == 0xc0feU)
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define IN6_IS_ADDR_LINKLOCAL(ap)                                       \
    (*(uint16_t *)(&(ap)->s6_addr[0]) & 0xffc0U == 0xfe80U)
#define IN6_IS_ADDR_SITELOCAL(ap)                                       \
    (*(uint16_t *)(&(ap)->s6_addr[0]) & 0xffc0U == 0xfec0U)
#endif /* __BYTE_ORDER */

#define __IN6_ADDR_SCOPE_NODELOCAL    0x01
#define __IN6_ADDR_SCOPE_INTFACELOCAL 0x01
#define __IN6_ADDR_SCOPE_LINKLOCAL    0x02
#define __IN6_ADDR_SCOPE_SITELOCAL    0x05
#define __IN6_ADDR_SCOPE_ORGLOCAL     0x08
#define __IN6_ADDR_SCOPE_GLOBAL       0x0e
#define __IN6_ADDR_MC_SCOPE(ap)       ((ap)->s6_addr[1] & 0x0f)

#define IN6_IS_ADDR_MC_NODELOCAL(ap)                                    \
    (IN6_IS_ADDR_MULTICAST(ap)                                          \
     && (__IN6_ADDR_MC_SCOPE(ap) == __IN6_ADDR_SCOPE_NODELOCAL))
#define IN6_IS_ADDR_MC_LINKLOCAL(ap)                                    \
    (IN6_IS_ADDR_MULTICAST(ap)                                          \
     && (__IN6_ADDR_MC_SCOPE(ap) == __IN6_ADDR_SCOPE_LINKLOCAL))
#define IN6_IS_ADDR_MC_SITELOCAL(ap)                                    \
    (IN6_IS_ADDR_MULTICAST(ap)                                          \
     && (__IN6_ADDR_MC_SCOPE(ap) == __IN6_ADDR_SCOPE_SITELOCAL))
#define IN6_IS_ADDR_MC_ORGLOCAL(ap)                                     \
    (IN6_IS_ADDR_MULTICAST(ap)                                          \
     && (__IN6_ADDR_MC_SCOPE(ap) == __IN6_ADDR_SCOPE_ORGLOCAL))
#define IN6_IS_ADDR_MC_GLOBAL(ap)                                       \
    (IN6_IS_ADDR_MULTICAST(ap)                                          \
     && (__IN6_ADDR_MC_SCOPE(ap) == __IN6_ADDR_SCOPE_GLOBAL))

#if (LONGSIZE == 8 || LONGLONGSIZE == 8)

#define IN6_ARE_ADDR_EQUAL(ap1, ap2)                                    \
    (*(uint64_t *)(&(ap1)->s_addr[0]) == *(uint64_t *)(&(ap2)->s_adr[0]) \
     && *(uint64_t *)(&(ap1)->s_addr[8]) == *(uint64_t *)(&(ap2)->s_adr[8]))

#elif (LONGSIZE == 4)

#define IN6_ARE_ADDR_EQUAL(ap1, ap2)                                    \
    (*(uint32_t *)(&(ap1)->s_addr[0]) == *(uint32_t *)(&(ap2)->s_adr[0]) \
     && *(uint32_t *)(&(ap1)->s_addr[4]) == *(uint32_t *)(&(ap2)->s_adr[4]))
     && *(uint32_t *)(&(ap1)->s_addr[8]) == *(uint32_t *)(&(ap2)->s_adr[8]))
     && *(uint32_t *)(&(ap1)->s_addr[12]) == *(uint32_t *)(&(ap2)->s_adr[12]))

#endif /* LONGSIZE */

#endif /* __NETINET_BITS_IN_H__ */

