#ifndef SYS_V0_CONF_H
#define SYS_V0_CONF_H

/*
 * V0_PLATFORM_TYPE             - build target architecture
 * - V0_PAPILIO_PRO             - Gadget Factory Papilio Pro Spartan 6 LX9
 */
#define V0_PLATFORM_TYPE        V0_PAPILIO_PRO
#define V0_PAPILIO_PRO          1
#define V0_THREADS              1

/*
 * V0_TARGET_CONF               - target configuration
 * - V0_FAST_RESPONSE_CONF      - low latency, fast response desktop
 * - V0_DESKTOP_CONF            - general-use desktop
 * - V0_SERVER_CONF             - server host configuration
 */
#define V0_TARGET_CONF          V0_FAST_RESPONSE_CONF
#define V0_FAST_RESPONSE_CONF   0
#define V0_DESKTOP_CONF         1
#define V0_SERVER_CONF          2

/* extensions; comment out or #undef to disable */
#define V0_MM_EXTENSION         1
#define V0_MP_EXTENSION         1
#define V0_FP_EXTENSION         1
#define V0_FX_EXTENSION         1
#if defined(V0_FP_EXTENSION)
#define V0_FP_IEEE_754          1
#endif

/* enable printing with documentation included */
// #define V0_LITERAL_DOCUMENTATION        1

#if (V0_TARGET_CONF == V0_FAST_RESPONSE_CONF)
#define V0_TMR_HZ               128
#define V0_TMR_SLICE_MS         4
#elif (V0_TARGET_CONF ==_DESKTOP_CONF)
#define V0_TMR_HZ               64
#define V0_TMR_SLICE_MS         64
#elif (V0_TARGET_CONF ==_SERVER_CONF)
#define V0_TMR_HZ               16
#define V0_TMR_SLICE            32
#else
#errof V0_TARGET_CONF not specified in <v0/conf.h>
#endif
#define V0_WORD_SIZE            4               // native user register size
#define V0_MAX_UNITS            16              // max # of unit IDs
#define V0_MAX_INSTS            64              // max # of unit instruction IDs
#define _MEGABYTE               (1024U * 1024U)
#define V0_INTR_VECTOR          0x00000000      // system interrupt vector
#define V0_CODE_ADR             V0_PAGE_SIZE    // system text segment address
#define V0_CODE_PROT_SIZE       (3 * V0_PAGE_SIZE) // protector maximum size
#define V0_SYS_STACK            0x00000000         // system stack top
#define V0_STACK_SIZE           (3 * V0_PAGE_SIZE) // per-thread stack size
#define V0_STACK_PROT_SIZE      V0_PAGE_SIZE       // protector maximum size
#if defined(V0_THREADS)
#define V0_PROC_THREADS         16                 // max # of process threads
#define V0_MAX_THREADS          256                // max # of system threads
#define V0_THR_ID_BITS          8                  // # of bits in thread-IDs
#endif

#define V0_FB_ADR               0xe0000000         // 3.5 gigabytes

/* optional features */
#define V0_PTHREAD              1                  // POSIX-thread support

#if !defined(V0_RELEASE)
#   define V0_SANITY_CHECK      1
#endif

#define V0_BOARD_TYPE           V0_PAPILIO_PRO
#define V0_BOARD_NONE           0
#define V0_PAPILIO_PRO          1

#define V0_DEF_COPROC_MASK      (V0_FXP_COPROC_BIT                      \
                                 | V0_FPU_COPROC_MASK)
#define V0_FXP_COPROC           0
#define V0_FPU_COPROC_BIT       1
#define V0_DSP_COPROC_BIT       2
#define V0_FXP_COPROC_BIT       (V0_FXP_COPROC << V0_UNIT_SHIFT)
#define V0_FPU_COPROC_BIT       (V0_FPU_COPROC << V0_UNIT_SHIFT)
#define V0_DSP_COPROC_BIT       (V0_DSP_COPROC << V0_UNIT_SHIFT)

#define V0_THREADS              1

#if (V0_BOARD_TYPE == V0_PAPILI0_PRO)
#include <v0/bsp/papilio-pro.h>
/* FIXME */
#define V0_RAM_SIZE             PAPILIO_PRO_RAM_SIZE
#else
#error V0_BOARD_TYPE not specified in <v0/conf.h>
#endif

#define V0_MAX_UNITS            16              // max # of unit IDs
#define V0_MAX_INSTS            64              // max # of unit instruction IDs
#define _MEGABYTE               (1024U * 1024U)
#define V0_INTR_VECTOR          0x00000000      // system interrupt vector
#define V0_CODE_ADR             V0_PAGE_SIZE    // system text segment address
#define V0_CODE_PROT_SIZE       (3 * V0_PAGE_SIZE) // protector maximum size
#define V0_SYS_STACK            0x00000000         // system stack top
#define V0_STACK_SIZE           (3 * V0_PAGE_SIZE) // per-thread stack size
#define V0_STACK_PROT_SIZE      V0_PAGE_SIZE       // protector maximum size
#if defined(V0_THREADS)
#define MACH_MAX_PROCS          MACH_MAX_THREADS
#define MACH_PROC_THREADS       16                 // max # of process threads
#define MACH_MAX_THREADS        256                // max # of system threads
#define MACH_THR_ID_BITS        8                  // # of bits in thread-IDs
#endif

#define MACH_FB_ADR             0xe0000000         // 3.5 gigabytes

/* optional features */
#define V0_PTHREAD              1                  // POSIX-thread support

#endif /* SYS_V0_CONF_H */

