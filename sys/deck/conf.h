#ifndef DECK_CONF_H
#define DECK_CONF_H

/*
 * V0_PLATFORM_TYPE             - build target architecture
 * - Vo_PAPILIO_PRO             - Gadget Factory Papilio Pro Spartan 6 LX9
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
#define DECK_TARGET_CONF        V0_FAST_RESPONSE_CONF
#define DECK_FAST_RESPONSE_CONF 0
#define DECK_DESKTOP_CONF       1
#define DECK_SERVER_CONF        2

/* enable printing with documentation included */
// #define V0_LITERAL_DOCUMENTATION        1

#if (DECK_TARGET_CONF ==_FAST_RESPONSE_CONF)
#define V0_TMR_HZ               128
#define V0_TMR_SLICE_MS         4
#elif (DECK_TARGET_CONF ==_DESKTOP_CONF)
#define V0_TMR_HZ               64
#define V0_TMR_SLICE_MS         64
#elif (DECK_TARGET_CONF ==_SERVER_CONF)
#define V0_TMR_HZ               16
#define V0_TMR_SLICE            32
#else
#errof DECK_TARGET_CONF not specified in <v0/conf.h>
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

#endif /* DECK_CONF_H */

