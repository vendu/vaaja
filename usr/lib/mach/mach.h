#ifndef MACH_MACH_H
#define MACH_MACH_H

#include <stdint.h>

/*
 * NOTES
 * -----
 * - our atomic operations such as m_cmpsetbit() use bus-locks and barriers
 */

/*
 * ATOMIC OPERATIONS
 * -----------------
 * m_atominc(p)                 - atomic increment [by one]; no return value
 * m_atomdec(p)                 - atomic decrement [by one]; no return value
 * m_atomswap(p, val)           - atomic swap of *p by val; return earlier value
 * m_fetchadd(p, val)           - atomic addition of val to *p; return earlier value
 * m_fetchswap(p, want, val)    - atomic CAS (compare and swap)
 *                                if (*p == want) *p = val; return earlier value
 * m_fetchswapu(p, want, val)   - unsigned CAS if (*p == want) *p = val
 * m_fetchswapptr(p, want, val) - atomic compare and swap of pointers
 * m_cmpswap(p, want, val)      - atomic CAS if (*p == want) *p = val; true on success
 * m_cmpswapu(p, want, val)     - unsigned CAS if (*p == want) *p = val;
 * m_cmpswapptr(p, want, val)   - pointer CAS if (*p == want) *p = val;
 * m_cmpswapdbl(p, want, val)   - dual-pointer CAS2; return true on success
 * m_setbit(p, ndx)             - atomic set bit (1L << ndx)
 * m_clrbit(p, ndx)             - atomic clear bit (1L << ndx)
 * m_flipbit(p, ndx)            - atomic flip (toggle) bit (1L << ndx)
 * m_cmpsetbit(p, ndx)          - if !(*p & (1L << ndx)) *p |= 1L << ndx; true if OK
 * m_cmpclrbit(p, ndx)          - if (*p & (1L << ndx)) *p &= ~(1L << ndx);
 * m_scanlo1bit(l)              - find lowest position with 1-bit
 * m_scanhi1bit(l)              - find highest position with 1-bit
 */

#if defined(__STDC__)
typedef void                   *m_ptr_t;
#else
typedef char                   *m_ptr_t;
#endif

#endif /* MACH_MACH_H */

