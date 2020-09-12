#ifndef __SYS_ZEN_BSP_V0_SEG_H__
#define __SYS_ZEN_BSP_V0_SEG_H__

/* process memory layout */

/* interrupt vector at address/page 0 */
#define ZEN_TRAP_VECTOR         0x00000000
/* system-mode stack size located just under 3.5G in virtual memory */
#define ZEN_SYS_STACK_BASE      0xe0000000
#define ZEN_SYS_STACK_SIZE      ZEN_PAGE_SIZE
/* user-mode stack size */
#define ZEN_USR_STACK_SIZE      (4 * ZEN_PAGE_SIZE)
#define ZEN_TEXT_MEM            ZEN_PAGE_SIZE
#define ZEN_USR_STACK_LIM       (ZEN_USR_STACK - ZEN_USR_STACK_SIZE)
#define ZEN_USR_STACK           (ZEN_SYS_STACK - ZEN_SYS_STACK_SIZE)
#define ZEN_SYS_STACK_LIM       (ZEN_SYS_STACK - ZEN_SYS_STACK_SIZE)
#define ZEN_SYS_STACK           ZEN_RAM_SIZE

struct memseg {
    uintptr_t                   adr;
    size_t                      size;
};

#endif /* #ifndef __SYS_ZEN_BSP_V0_SEG_H__ */

