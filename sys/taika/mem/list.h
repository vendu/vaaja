#ifndef SYS_TAIKA_MEM_LIST_H
#define SYS_TAIKA_MEM_LIST_H

#include <mach/param.h>
#include <kern/conf.h>
#include <zero/trix.h>

#if defined(MEM_CONST_SIZE_TRICK)
#define memcalclist(sz)         __memfastlist(sz)
#else
#define memcalclist(sz)         __memcalclist(sz)
#endif

#define MEMLISTHDRSIZE          MACH_CL_SIZE
struct memlist {
    MEM_LK_T                    lk;
    void                       *list;
};

static __inline__ unsigned long
__memcalclist(unsigned long size)
{
    unsigned long               list = MACH_PTR_BITS;
    unsigned long               nlz;

    if (size) {
        nlz = __builtin_clzl(size);
        list -= nlz;
        if (powerof2(size)) {
            list--;
        }
    }

    return list;
}

/* use compiler optimizations to evaluate bucket for constant allocation size */
#if (MEM_CONST_SIZE_TRICK)
#if (MACH_PTR_BITS <= 32)
#define __memfastlist(sz)                                                \
    ((!isconst(sz)                                                      \
      ? __memcalclist(sz)                                                \
      : (((sz) <= MEMMINSIZE)                                           \
         ? MEMMINSHIFT                                                  \
         : (((sz) <= (1UL << 4))                                        \
            ? 4                                                         \
            : (((sz) <= (1UL << 5))                                     \
               ? 5                                                      \
               : (((sz) <= (1UL << 6))                                  \
                  ? 6                                                   \
                  : (((sz) <= (1UL << 7))                               \
                     ? 7                                                \
                     : (((sz) <= (1UL << 8))                            \
                        ? 8                                             \
                        : (((sz) <= (1UL << 9))                         \
                           ? 9                                          \
                           : (((sz) <= (1UL << 10))                     \
                              ? 10                                      \
                              : (((sz) <= (1UL << 11))                  \
                                 ? 11                                   \
                                 : (((sz) <= (1UL << 12))               \
                                    ? 12                                \
                                    : (((sz) <= (1UL << 13))            \
                                       ? 13                             \
                                       : (((sz) <= (1UL << 14))         \
                                          ? 14                          \
                                          : (((sz) <= (1UL << 15))      \
                                             ? 15                       \
                                             : (((sz) <= (1UL << 16))   \
                                                ? 16                    \
                                                : (((sz) <= (1UL << 17)) \
                                                   ? 17                 \
                                                   : (((sz) <= (1UL << 18)) \
                                                      ? 18              \
                                                      : (((sz) <= (1UL << 19) \
                                                          ? 19          \
                                                          : (((sz) <= (1UL << 20)) \
                                                             ? 20       \
                                                             : (((sz) <= (1UL << 21)) \
                                                                ? 21    \
                                                                : (((sz) <= (1UL << 22)) \
                                                                   ? 22 \
                                                                   : (((sz) <= (1UL << 23)) \
                                                                      ? 23 \
                                                                      : (((sz) <= (1UL << 24)) \
                                                                         ? 24 \
                                                                         : (((sz) <= (1UL << 25)) \
                                                                            ? 25 \
                                                                            : (((sz) <= (1UL << 26)) \
                                                                               ? 26 \
                                                                               : (((sz) <= (1UL << 27)) \
                                                                                  ? 27 \
                                                                                  : (((sz) <= (1UL << 28)) \
                                                                                     ? 28 \
                                                                                     : (((sz) <= (1UL << 29)) \
                                                                                        ? 29 \
                                                                                        : (((sz) <= (1UL << 30)) \
                                                                                           ? 30 \
                                                                                           : (((sz) <= (1UL << 31)) \
                                                                                              ? 31 \
                                                                                              : 0xff))))))))))))))))))))))))))))))))
#elif (MACH_PTR_BITS <= 64)
#define memfastlist(sz)                                                  \
    ((!isconst(sz)                                                      \
      ? __memcalclist(sz)                                                \
      : (((sz) <= MEMMINSIZE)                                           \
         ? MEMMINSHIFT                                                  \
         : (((sz) <= (UINT64_C(1) << 4))                                        \
            ? 4                                                         \
            : (((sz) <= (UINT64_C(1) << 5))                                     \
               ? 5                                                      \
               : (((sz) <= (UINT64_C(1) << 6))                                  \
                  ? 6                                                   \
                  : (((sz) <= (UINT64_C(1) << 7))                               \
                     ? 7                                                \
                     : (((sz) <= (UINT64_C(1) << 8))                            \
                        ? 8                                             \
                        : (((sz) <= (UINT64_C(1) << 9))                         \
                           ? 9                                          \
                           : (((sz) <= (UINT64_C(1) << 10))                     \
                              ? 10                                      \
                              : (((sz) <= (UINT64_C(1) << 11))                  \
                                 ? 11                                   \
                                 : (((sz) <= (UINT64_C(1) << 12))               \
                                    ? 12                                \
                                    : (((sz) <= (UINT64_C(1) << 13))            \
                                       ? 13                             \
                                       : (((sz) <= (UINT64_C(1) << 14))         \
                                          ? 14                          \
                                          : (((sz) <= (UINT64_C(1) << 15))      \
                                             ? 15                       \
                                             : (((sz) <= (UINT64_C(1) << 16))   \
                                                ? 16                    \
                                                : (((sz) <= (UINT64_C(1) << 17)) \
                                                   ? 17                 \
                                                   : (((sz) <= (UINT64_C(1) << 18)) \
                                                      ? 18              \
                                                      : (((sz) <= (UINT64_C(1) << 19) \
                                                          ? 19          \
                                                          : (((sz) <= (UINT64_C(1) << 20)) \
                                                             ? 20       \
                                                             : (((sz) <= (UINT64_C(1) << 21)) \
                                                                ? 21    \
                                                                : (((sz) <= (UINT64_C(1) << 22)) \
                                                                   ? 22 \
                                                                   : (((sz) <= (UINT64_C(1) << 23)) \
                                                                      ? 23 \
                                                                      : (((sz) <= (UINT64_C(1) << 24)) \
                                                                         ? 24 \
                                                                         : (((sz) <= (UINT64_C(1) << 25)) \
                                                                            ? 25 \
                                                                            : (((sz) <= (UINT64_C(1) << 26)) \
                                                                               ? 26 \
                                                                               : (((sz) <= (UINT64_C(1) << 27)) \
                                                                                  ? 27 \
                                                                                  : (((sz) <= (UINT64_C(1) << 28)) \
                                                                                     ? 28 \
                                                                                     : (((sz) <= (UINT64_C(1) << 29)) \
                                                                                        ? 29 \
                                                                                        : (((sz) <= (UINT64_C(1) << 30)) \
                                                                                           ? 30 \
                                                                                           : (((sz) <= (UINT64_C(1) << 31)) \
                                                                                              ? 31 \
                                                                                              : (((sz) <= (UINT64_C(1) << 32)) \
                                                                                                 ? 32 \
                                                                                                 : (((sz) <= (UINT64_C(1) << 33)) \
                                                                                                    ? 33 \
                                                                                                    : (((sz) <= (UINT64_C(1) << 34)) \
                                                                                                       ? 34 \
                                                                                                       : (((sz) <= (UINT64_C(1) << 35)) \
                                                                                                          ? 35 \
                                                                                                          :(((sz) <= (UINT64_C(1) << 36)) \
                                                                                                            ? 36 \
                                                                                                            : (((sz) <= (UINT64_C(1) << 37)) \
                                                                                                               ? 37 \
                                                                                                               : (((sz) <= (UINT64_C(1) << 38)) \
                                                                                                                  ? 38 \
                                                                                                                  : (((sz) <= (UINT64_C(1) << 39)) \
                                                                                                                     ? 39 \
                                                                                                                     : (((sz) <= (UINT64_C(1) << 40)) \
                                                                                                                        ? 40 \
                                                                                                                        : (((sz) <= (UINT64_C(1) << 41)) \
                                                                                                                           ? 41 \
                                                                                                                           : (((sz) <= (UINT64_C(1) << 42)) \
                                                                                                                              ? 42 \
                                                                                                                              : (((sz) <= (UINT64_C(1) << 43)) \
                                                                                                                                 ? 43 \
                                                                                                                                 : (((sz) <= (UINT64_C(1) << 44)) \
                                                                                                                                    ? 44 \
                                                                                                                                    : (((sz) <= (UINT64_C(1) << 45)) \
                                                                                                                                       ? 45 \
                                                                                                                                       : (((sz) <= (UINT64_C(1) << 46)) \
                                                                                                                                          ? 46 \
                                                                                                                                          : (((sz) <= (UINT64_C(1) << 47)) \
                                                                                                                                             ? 47 \
                                                                                                                                             : (((sz) <= (UINT64_C(1) << 48)) \
                                                                                                                                                ? 48 \
                                                                                                                                                : (((sz) <= (UINT64_C(1) << 49)) \
                                                                                                                                                   ? 49 \
                                                                                                                                                   : (((sz) <= (UINT64_C(1) << 50)) \
                                                                                                                                                      ? 50 \
                                                                                                                                                      : (((sz) <= (UINT64_C(1) << 51)) \
                                                                                                                                                         ? 51 \
                                                                                                                                                         : (((sz) <= (UINT64_C(1) << 52)) \
                                                                                                                                                            ? 52 \
                                                                                                                                                            : (((sz) <= (UINT64_C(1) << 53)) \
                                                                                                                                                               ? 53 \
                                                                                                                                                               : (((sz) <= (UINT64_C(1) << 54)) \
                                                                                                                                                                  ? 54 \
                                                                                                                                                                  : (((sz) <= (UINT64_C(1) << 55)) \
                                                                                                                                                                     ? 55 \
                                                                                                                                                                     : (((sz) <= (UINT64_C(1) << 56)) \
                                                                                                                                                                        ? 56 \
                                                                                                                                                                        : (((sz) <= (UINT64_C(1) << 57)) \
                                                                                                                                                                           ? 57 \
                                                                                                                                                                           : (((sz) <= (UINT64_C(1) << 58)) \
                                                                                                                                                                              ? 58 \
                                                                                                                                                                              : (((sz) <= (UINT64_C(1) << 59)) \
                                                                                                                                                                                 ? 59 \
                                                                                                                                                                                 : (((sz) <= (UINT64_C(1) << 60)) \
                                                                                                                                                                                    ? 60 \
                                                                                                                                                                                    : (((sz) <= (UINT64_C(1) << 61)) \
                                                                                                                                                                                       ? 61 \
                                                                                                                                                                                       : (((sz) <= (UINT64_C(1) << 62)) \
                                                                                                                                                                                          ? 62 \
                                                                                                                                                                                          : (((sz) <= (UINT64_C(1) << 63)) \
                                                                                                                                                                                             ? 63 \
                                                                                                                                                                                             : 0xff))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
#endif /* MACH_PTR_BITS */
#endif /* defined(MEM_CONST_SIZE_TRICK) */

#endif /* SYS_TAIKA_MEM_LIST_H */

