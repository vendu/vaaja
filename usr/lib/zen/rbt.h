#ifndef __ZERO_RBT_H__
#define __ZERO_RBT_H__

#include <stdint.h>

#define RBT_DATA_COLOR   1
#define RBT_UINTPTR_DATA 1
#define RBT_INTPTR_DATA  0
#define RBT_BLACK        0
#define RBT_RED          1
#define RBT_PREORDER     0
#define RBT_INORDER      1
#define RBT_POSTORDER    2

#if defined(RBT_INTPTR_DATA)
#define rbtcmpkeys(k1, k2) ((k1) - (k2))
#elif defined(RBT_UINTPTR_DATA)
#define rbtcmpkeys(k1, k2) ((k1) < (k2) ? -1 : 1)
#endif

#if defined(RBT_DATA_COLOR)
#define RBT_DATA_COLOR_BIT ((intptr_t)1 << 0)
#define rbtsetcolor(np, c) ((np)->data |= (c))
#define rbtgetcolor(np)    ((np)->data & RBT_DATA_COLOR_BIT)
#define rbtclrcolor(np)    ((np)->data & ~RBT_DATA_COLOR_BIT)
#else
#define rbtsetcolor(np, c) ((np)->color = (c))
#define rbtgetcolor(np, c) ((np)->color)
#endif

#define rbtisempty(tp)                                                  \
    ((tp)->root.left == &(tp)->nil && (tp)->root.right == &(tp)->nil)

struct rbtnode {
#if defined(RBT_INTPTR_DATA)
    intptr_t        key;
    intptr_t        data;
#elif defined(RBT_UINTPTR_DATA)
    untptr_t        key;
    uintptr_t       data;
#else
    void           *data;
    long            color;
#endif
    struct rbtnode *parent;
    struct rbtnode *left;
    struct rbtnode *right;
};

struct rbt {
#if !defined(RBT_INTPTR_DATA) && !defined(RBT_UINTPTR_DATA)
    int            (*cmp)(const void *, const void *);
#endif
    struct rbtnode   root;
    struct rbtnode   nil;
};

#endif /* __ZERO_RBT_H__ */

