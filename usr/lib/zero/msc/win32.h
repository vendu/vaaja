#ifndef __ZERO_MSC_WIN32_H__
#define __ZERO_MSC_WIN32_H__

#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

#define m_membar()               _ReadWriteBarrier()
#define m_memrdbar()             _ReadBarrier()
#define m_memwrbar()             _WriteBarrier()
#define m_cmpswap(p, want, val)  _InterlockedCompareExchange(p, val, want))
#define m_cmpswapb(p, want, val) _InterlockedCompareExchange8(p, val, want))
#define m_scanlo1bit(l)          _BitScanForward(&(l), (l)))
#define m_scanhi1bit(l)          _BitScanReverse(&(l), (l)))
#define m_getretadr(pp)          (*(pp) = _ReturnAddress())
#define m_setretadr(pp)          (*(pp) = _ReturnAddress())

#endif /* __ZERO_MSC_WIN32_H__ */

