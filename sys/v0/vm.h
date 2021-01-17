#ifndef SYS_V0_VM_H
#define SYS_V0_VM_H

/* v0 virtual machine */

#define V0_VM_MEM_DEF_SIZE    (8 * 1024 * 1024)   // 8 megabytes

#define V0_VM_RESET             0x00
#define V0_VM_RUN               0x01
#define V0_VM_STOP              0x02
#define V0_VM_HALT              0x03

struct v0vm {
    int8_t                     *mem;        // virtual memory
    uint32_t                    memsize;    // size of memory in bytes
    int8_t                     *bram;       // block RAM
    uint32_t                    bramsize;   // size of BRAM in 2048-byte units
    int8_t                     *dram;       // SDRAM memory
    uint32_t                    dramsize;   // size of SDRAM in bytes
    uint32_t                   *irqvec;
    uint32_t                    iregs[V0_INT_REGS];
    uint8_t                     state;      // RESET, RUN, STOP, HALT
};

#endif /* SYS_V0_VM_H */

