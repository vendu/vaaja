#ifndef __SYS_ZEN_STYLE_H__
#define __SYS_ZEN_STYLE_H__

#include <zero/mtx.h>

/*
 * - macros are wrapped inside a do-while block so as do { stuff(); } while (0);
 * - most aggregate types, i.e. structures and unions, do not have corresponding
 *   typedefs, but the aggregate-nature of such types is shown explicitly to
 *   ease understanding of the code
 * - library types may use
 *
 * typedef struct __stkitem {
 *     uintptr_t         val;
 *     struct __stkitem *next;
 * } zerostkitem;
 */
struct stkitem {
    uintptr_t       val;
    struct stkitem *next;
};

#define stkpush(stk, item)                                              \
    do {                                                                \
        mtxlk(&(stk)->lk);                                              \
        (item)->next = (stk)->top;                                      \
        (stk)->top = (item);                                            \
        mtxunlk(&(stk)->lk);                                            \
    } while (0)

struct stk {
    volatile long   lk;
    struct stkitem *top;
};

#endif /* __SYS_ZEN_STYLE_H__ */

