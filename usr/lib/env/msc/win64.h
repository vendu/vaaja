#ifndef __ZERO_MSC_WIN64_H__
#define __ZERO_MSC_WIN64_H__

#include <intrin.h>
#include <msc/win32.h>

#define m_cmpswap64(p, want, val) _InterLockedCompareExchange64(p, val, want)
#define m_scanlo1bit(ll)          _BitScanForward64(&(ll), (ll)))
#define m_scanhi1bit(ll)          _BitScanReverse64(&(ll), (ll)))

#endif /* __ZERO_MSC_WIN64_H__ */

