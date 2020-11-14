#ifndef __VOIMA_VM_TYPES_H__
#define __VOIMA_VM_TYPES_H__

/* status-flags */
#define V0_VM_READY         (1 << 31)
#define V0_VM_TERMINATE     (1 << 30)
#define V0_VM_RESET         (1 << 29)
#define v0vmexitstat(vm)    ((vm)->status & 0xff)
struct v0vm {
    volatile m_atomic_t     mtx;
    volatile m_atomic_t     buslk;
    volatile long           status;
    volatile void          *bram;
    size_t                  bramsize;
    volatile void          *mem;
    size_t                  memsize;
    struct v0thr           *thr;
    volatile int32_t        intregs[V0_INTEGER_REGISTERS];
    volatile int32_t        sysregs[V0_SYSTEM_REGISTERS];
};

#endif /* __VOIMA_VM_TYPES_H__ */

