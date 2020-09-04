#ifndef __VOIMA_TRAP_H__
#define __VOIMA_TRAP_H__

#define VOIMA_TMR_TRAP          0x00    // timer interrupt
// err is interrupt ID
#define VOIMA_BP_TRAP           0x01    // breakpoint or single-step
// err is PC for debugged instruction
#define VOIMA_PF_TRAP           0x02     // page-fault exceptions
// err is page-fault address + SYS | WR | RD bits
#define VOIMA_SYS_TRAP          0x03    // invalid system call
#define VOIMA_ILL_TRAP          0x04    // invalid opcode/instruction SIGILL
#define VOIMA_INV_TRAP          0x05    // invalid argument for operation
#define VOIMA_DB_TRAP           0x05    // debug trap
#define VOIMA_MEM_TRAP          0x06    // SIGBUS; SIGSEGV
#define VOIMA_STK_TRAP          0x07    // SIGSTKFLT
#define VOIMA_STK_IO_TRAP       0x08    // urgent traffic

#endif /* __VOIMA_TRAP_H__ */

