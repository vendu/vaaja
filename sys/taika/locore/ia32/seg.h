#ifndef __UNIT_IA32_SEG_H__
#define __UNIT_IA32_SEG_H__

#include <stdint.h>

extern

#define segsetdesc(p, adr, lim, flg)                                    \
    do {                                                                \
        uint64_t _adr = (uintptr_t)(adr);                               \
        uint64_t _lim = (uintptr_t)(lim);                               \
                                                                        \
        *(p) = _mksegdesc(_adr, _lim, flg);                             \
    } while (0)

#define _seglim(lim)                                                    \
    ((((lim)  & UINT64_C(0x000f0000)) << 32)                            \
     | ((lim) & UINT64_C(0x0000ffff)))
#define _segadr(adr)                                                    \
    ((((adr) & UINT64_C(0xff000000)) << 32)                             \
     | ((((adr) & UINT64_C(0x00ff0000))) << 16)                         \
     | ((((adr) & UINT64_C(0x0000ffff))) << 16))
#define _mksegdesc(adr, npg, flg)                                       \
    (_segadr(adr)                                                       \
     | _seglim(npg)                                                     \
     | (flg))

/* segment descriptor setup */

/* convenience macros */
#define SEGCODE     (SEGNORM | SEGDEFBITS | SEGEXEC)
#define SEGDATA     (SEGNORM | SEGDEFBITS | SEGWRITE)
#define SEGSTK      (SEGNORM | SEGDEFBITS | SEGWRITE | SEGDOWN)
#define SEGCPU      (SEGNORM | SEGWRITE | SEGPRES)
/* acs field */
#define SEGBUSY     UINT64_C(0x0000010000000000)
#define SEGWRITE    UINT64_C(0x0000020000000000)
#define SEGDOWN     UINT64_C(0x0000040000000000)
#define SEGEXEC     UINT64_C(0x0000080000000000)
#define SEGAVAILTSS UINT64_C(0x0000090000000000)
#define SEGISTMASK  UINT64_C(0x0000000700000000)
//#define SEGBUSYTSS  UINT64_C(0x00000b0000000000)
//#define SEGLDT      UINT64_C(0x0000020000000000)
#define SEGNORM     UINT64_C(0x0000100000000000)
#define SEGUSER     UINT64_C(0x0000600000000000)
#define SEGPRES     UINT64_C(0x0000800000000000)
/* flg field */
#define SEG32BIT    UINT64_C(0x0040000000000000)
#define SEG4KGRAN   UINT64_C(0x0080000000000000)
/* type field */

#define SEGDEFBITS (SEG32BIT | SEG4KGRAN | SEGPRES)
#define SEGTSS     (SEGAVAILTSS | SEGUSER | SEGPRES)

#endif /* __UNIT_IA32_SEG_H__ */

