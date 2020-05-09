#ifndef __MACH_X86_TYPES_H_
#define __MACH_X86_TYPES_H_

#include <stdint.h>

#if 0
typedef uintptr_t pde_t;
typedef uintptr_t pte_t;
#endif

/* far pointer structure */
#include <zero/pack.h>
struct m_farptr {
    uint16_t lim;
    uint32_t adr;
} PACKED();
#include <zero/nopack.h>

/* FPU registers */

struct m_fpreg {
    int16_t _sig[4];
    int16_t _exp;
};

struct m_fpxreg {
    int16_t _sig[4];
    int16_t _exp;
    int16_t _pad[3];
};

struct m_xmmreg {
    int32_t _elem[4];
};

#define FPX_MAGIC1      0x46505853U
#define FPX_MAGIC2      0x46505845U
#define FPX_MAGIC2_SIZE sizeof(FPX_MAGIC2)

struct m_fpdata {
    int32_t _magic1;    // FPX_MAGIC1
    int32_t _extsize;
    int64_t _xstate;
    int32_t _xsize;
    int32_t _pad[7];
};

/* floating-point state */
#define X86_FXSR_MAGIC 0x0000
struct m_fpstate32 {
    int32_t             _cw;
    int32_t             _sw;
    int32_t             _tag;
    int32_t             _ipofs;
    int32_t             _cs;
    int32_t             _dataofs;
    int32_t             _ds;
    struct m_fpreg      _st[8];
    int16_t             _status;
    int16_t             _magic;  // 0xffff means regular FPU data only
    /* FXSR FPU environment */
    int32_t             _fxsrenv[6];
    int32_t             _mxcsr;
    int32_t             _res;
    struct m_fpxreg     _fxsr[8];
    struct m_xmmreg     _xmm[8];
    int32_t             _pad1[44];
    union {
        int32_t         _pad2[12];
        struct m_fpdata _swres;
    } u;
};

#endif /* __MACH_X86_TYPES_H_ */

