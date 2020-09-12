#ifndef __SYS_ZEN_BSP_V0_MACH_H__
#define __SYS_ZEN_BSP_V0_MACH_H__

#if ((defined(__v0__) || defined(__i386__) || defined(__i486__)         \
      || defined(__i586__) || defined(__i686__))                        \
     && ((!defined(__x86_64__) && !defined(__amd64__)                   \
          && !defined(__ppc64__) && !defined(_WIN64))                   \
         || (defined(__ILP32__) || defined(__WIN32))))
#define __WORDSIZE              32
#else
#define __WORDSIZE              64
#endif

#if defined(__v0__)
#define __REGOPSIZE             32
#endif

#endif /* __SYS_ZEN_BSP_V0_MACH_H__ */

