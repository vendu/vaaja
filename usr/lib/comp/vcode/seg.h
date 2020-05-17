#ifndef __SYS_VCODE_SEG_H__
#define __SYS_VCODE_SEG_H__

/* segment flags */
#define V_USR_NULL_MASK 0x00    // no bits set (NULL-segment)
#define V_USR_EXEC_BIT  0x01    // user execute-permission
#define V_USR_WRITE_BIT 0x02    // user write-permission
#define V_USR_READ_BIT  0x04    // user read-permission
#define V_SYS_EXEC_BIT  0x08    // user execute-permission
#define V_SYS_WRITE_BIT 0x10    // user write-permission
#define V_SYS_READ_BIT  0x20    // user read-permission
#define V_SYS_MODE_BIT  0x40    // system mode required
#define V_WIRE_BIT      0x80    // unpaged memory region

/* virtual machine memory segments */
#define V_NULL          0x00    // all 1-bit page #0
#define V_CODE          0x01    // executable code
#define V_READ_ONLY     0x02    // read-only data
#define V_DATA          0x03    // [initialized] read-write data
#define V_HEAP          0x04    // dynamic-size allocated data
#define V_MAP_MEM       0x05    // memory-mapped [user] regions
#define V_USR_STACK     0x06    // user-mode stack
#define V_SYS_STACK     0x07    // system-mode stack
#define V_SYS_SEG_BIT   0x08    // system-segment flag
#define V_SYS_CODE      0x09    // system executable code
#define V_SYS_READ_ONLY 0x0a    // system read-only data
#define V_SYS_DATA      0x0b    // [initialized] system read-write data
#define V_SYS_HEAP      0x0c    // dynamic size allocated system data
#define V_SYS_MAP_MEM   0x0d    // system memory-mapped regions [devices]
#define V_SYS_USR_STACK 0x0e    // system user-mode execution stack
#define V_SYSTEM_STACK  0x0f    // system stack
#define V_INTR_VECTOR   0xff    // interrupt vector

#endif /* __SYS_VCODE_SEG_H__ */

