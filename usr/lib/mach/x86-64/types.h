#ifndef __MACH_X86_64_TYPES_H_
#define __MACH_X86_64_TYPES_H_

#include <stdint.h>
#include <mach/x86/types.h>

/* machine types */
typedef int64_t  m_reg_t;
typedef uint64_t m_ureg_t;

//typedef int64_t  m_atomic_t;

/* C call frame */
struct m_stkframe {
    /* automatic variables go here */
    int64_t fp;         // caller frame pointer
    int64_t pc;         // return address
    /* call parameters on stack go here in 'reverse order' */
};

/* general purpose registers - 32 bytes */
struct m_genregs {
    int64_t r8;
    int64_t r9;
    int64_t r10;
    int64_t r11;
    int64_t r12;
    int64_t r13;
    int64_t r14;
    int64_t r15;
    int64_t rdi;
    int64_t rsi;
    int64_t rbp;
    int64_t rbx;
    int64_t rdx;
    int64_t rax;
    int64_t rcx;
    int64_t rsp;
};

struct m_fpxstate64 {
    int16_t             _cw;
    int16_t             _sw;
    int16_t             _tag;
    int16_t             _fop;
    int64_t             _rip;
    int64_t             _rdp;
    int32_t             _mxcsr;
    int32_t             _mxcsrmask;
    struct m_fpxreg     _st[8];
    struct m_xmmreg     _xmm[16];
    int32_t             _res1[12];
    union {
        int32_t         _res2[12];
        struct m_fpdata _swres;
    } u;
};

struct m_fpxhdr64 {
    int64_t _state;
    int64_t _res1[2];
    int64_t _res2[5];
};

struct m_fpystate64 {
    int32_t _ymm[64];
};

struct m_fpstate64 {
    struct m_fpxstate64 _fpx;
    struct m_fpxhdr64   _fpxhdr;
    struct m_fpystate64 _fpy;
    /* extensions go here */
};

struct m_trapframe {
    int64_t rip;
    int64_t cs;
    int64_t rflags;
    int64_t ursp;
    int64_t ss;
};

/* thread control block */
struct m_tcb {
    struct m_genregs   genregs;
    struct m_trapframe trapframe;
};

struct m_ctx {
    int64_t            onstk;      // non-zero if on signal-stack
    int64_t            oldmask;    // signal mask to be restored
    struct m_tcb       tcb;        // task control block (machine context)
    int64_t            err;        // error code or zero
    int64_t            trapnum;    // # of trap
    int64_t            cr2;        // page-fault virtual address
    int64_t            fpflg;      // FPU flags
    int64_t            fpstatword; // FPU status word
    struct m_fpstate64 fpstate;    // FPU state
};

struct m_task {
    int64_t            flg;      // 8 bytes @ 0; task flags
    struct m_tcb       tcb;      // 168 bytes @ 8
    int64_t            trapesp;  // 8 bytes @ 176
    int8_t             _res[80]; // 80 bytes @ 256; pad to cacheline-boundary
    struct m_fpstate64 fpstate;  // X bytes @ 256; FPU state
};

#endif /* __MACH_X86_64_TYPES_H_ */

