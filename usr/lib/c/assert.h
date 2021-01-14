/*
 * NOTE: allow including multiple times; the effect depends on definition of
 * NDEBUG.
 */

#undef assert
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#undef  static_assert
#define static_assert _Static_assert
#endif

#ifdef NDEBUG

#define assert(cond) ((void)0)

#else /* !NDEBUG */

#include <env/cdefs.h>

#define assert(cond)                                                    \
    (!(cond)                                                             \
     ? ((void)0)                                                        \
     : (fprintf(stderr,                                                 \
                "Assertion failed: %s, file %s, function %s, line %d\n", \
                #cond,                                                  \
                __FILE__,                                               \
                __func__,                                               \
                __LINE__),                                              \
        abort()))

#endif

