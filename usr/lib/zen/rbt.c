/*
 * REFERENCES
 * ----------
 * https://opensource.apple.com/source/sudo/sudo-46/src/redblack.c
 * https://opensource.apple.com/source/sudo/sudo-46/src/redblack.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <zero/rbt.h>

extern struct rbtnode * rbtgetnode(void);
extern void             rbtputnode(struct rbtnode *);

struct rbt *
#if defined(RBT_INTPTR_DATA)
rbtinit(struct rbt *tree)
#else
rbtinit(long (*cmp)(const void *, const void *))
#endif
{
    struct rbtnode *nil;

    if (!tree) {
        tree = malloc(sizeof(struct rbt));
    }
    if (!tree) {
        fprintf(stderr, "rbt failed to allocated memory\n");

        exit(1);
    }
#if !defined(RBT_INTPTR_DATA) && !defined(RBT_UINTPTR_DATA)
    tree->cmp = cmp;
#endif
    nil = &tree->nil;
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINPTR_DATA)
#if defined(RBT_DATA_COLOR)
    nil->data = RBT_BLACK;
#else
    nil->data = 0;
    nil->color = RBT_BLACK;
#endif
#else
    nil->data = NULL;
    nil->color = RBT_BLACK;
#endif
    nil->parent = nil;
    nil->left = nil;
    nil->right = nil;
#if (defined(RBT_INTPTR_DATA) || defined(RBG_UINTPTR_DATA)) && defined(RBT_DATA_COLOR)
    tree->root.data = RBT_BLACK;
#else
    tree->root.data = NULL;
    tree->root.color = RBT_BLACK;
#endif
    tree->root.parent = nil;
    tree->root.left = nil;
    tree->root.right = nil;

    return tree;
}

struct rbtnode *
rbtrotleft(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *child;

    child = node->right;
    node->right = child->left;
    if (child->left != &tree->nil) {
        child->left->parent = node;
    }
    child->parent = node->parent;
    if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->left = node;
    node->parent = child;

    return child;
}

struct rbtnode *
rbtrotright(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *child;

    child = node->left;
    node->left = child->right;
    if (child->right != &tree->nil) {
        child->right->parent = node;
    }
    child->parent = node->parent;
    if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->right = node;
    node->parent = child;

    return child;
}

void
rbtfix(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *parent = node->parent;
    struct rbtnode *uncle;

    while (rbtgetcolor(parent) == RBT_RED) {
        if (parent == parent->parent->left) {
            uncle = parent->parent->right;
            if (rbtgetcolor(uncle) == RBT_RED) {
#if defined(RBT_DATA_COLOR)
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(uncle, RBT_BLACK);
                rbtsetcolor(parent->parent, RBT_RED);
#else
                parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                parent->parent->color = RBT_RED;
#endif
                node = parent->parent;
                parent = node->parent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rbtrotleft(tree, node);
                }
#if defined(RBT_DATA_COLOR)
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(parent->parent, RBT_RED);
#else
                parent->color = RBT_BLACK;
                parent->parent->color = RBT_RED;
#endif
                parent = rbtrotright(tree, parent->parent);
            }
        } else {
            uncle = parent->parent->left;
            if (rbtgetcolor(uncle) == RBT_RED) {
#if defined(RBT_DATA_COLOR)
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(uncle, RBT_BLACK);
                rbtsetcolor(parent->parent, RBT_RED);
#else
                parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                parent->parent->color = RBT_RED;
#endif
                node = parent->parent;
                parent = node->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtrotright(tree, node);
                }
#if defined(RBT_DATA_COLOR)
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(parent->parent, RBT_RED);
#else
                parent->color = RBT_BLACK;
                parent->parent->color = RBT_RED;
#endif
                parent = rbtrotleft(tree, node->parent->parent);
            }
        }
    }
    rbtsetcolor(&tree->root, RBT_BLACK);

    return;
}

struct rbtnode *
#if defined(RBT_INTPTR_DATA)
rbtinsert(struct rbt *tree, intptr_t key, uintptr_t data)
#elif defined(RBT_UINTPTR_DATA)
rbtinsert(struct rbt *tree, uintptr_t key, uintptr_t data)
#else
rbtinsert(struct rbt *tree, void *data)
#endif
{
    struct rbtnode *node = &tree->root;
    struct rbtnode *parent = NULL;
    struct rbtnode *nil = &tree->nil;
    long            res;

    while (node != nil) {
        parent = node;
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
        res = rbtcmpkeys(key, node->key);
#else
        res = tree->cmp(data, node->data);
#endif
        if (!res) {

            return node;
        }
        if (res < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    node = rbtgetnode();
    if (!node) {
        fprintf(stderr, "rbt failed to allocated memory\n");

        exit(1);
    }
    node->key = key;
    node->data = data;
#if (defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)) && defined(RBT_DATA_COLOR)
    rbtsetcolor(node, RBT_RED);
#else
    node->color = RBT_RED;
#endif
    node->parent = parent;
    node->left = nil;
    node->right = nil;
    if (
        parent == &tree->root
#if !defined(RBT_INTPTR_DATA) && !defined(RBT_UINTPTR_DATA)
        || rbtcmpkeys(key, node->key)
#else
        || data < parent->data
#endif
            ) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    rbtfix(tree, node);

    return NULL;
}

struct rbtnode *
#if defined(RBT_INTPTR_DATA)
rbtfind(struct rbt *tree, intptr_t key)
#elif defined(RBT_INTPTR_DATA)
rbtfind(struct rbt *tree, uintptr_t key)
#else
rbtfind(struct rbt *tree, void *key)
#endif
{
    struct rbtnode *node = &tree->root;
    struct rbtnode *nil = &tree->nil;
    long            res;

    while (node != nil) {
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
        res  = rbtcmpkeys(key, node->key);
#else
        res = tree->cmp(key, node->key);
#endif
        if (!res) {

            return node;
        }
        if (res < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return NULL;
}

int
rbtapply(struct rbt *tree,
         struct rbtnode *node,
#if defined(RBT_INTPTR_DATA)
         long (*func)(intptr_t, intptr_t),
         intptr_t cookie,
#elif defined(RBT_UINTPTR_DATA)
         long (*func)(uintptr_t, uintptr_t),
         intptr_t cookie,
#else
         long (*func)(void *, void *),
         void *cookie,
#endif
         long order)
{
    long            error;

    if (node != &tree->nil) {
        if (order == RBT_PREORDER) {
            error = func(node->data, cookie);
            if (error) {

                return error;
            }
        }
        error = rbtapply(tree, node->left, func, cookie, order);
        if (error) {

            return error;
        }
        if (order == RBT_INORDER) {
            error = func(node->data, cookie);
            if (error) {

                return error;
            }
        }
        error = rbtapply(tree, node->right, func, cookie, order);
        if (error) {

            return error;
        }
        if (order == RBT_POSTORDER) {
            error = func(node->data, cookie);
            if (error) {

                return error;
            }
        }
    }

    return 0;
}

struct rbtnode *
rbtsuccessor(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *succ = node->right;
    struct rbtnode *nil = &tree->nil;

    if (succ != nil) {
        while (succ->left != nil) {
            succ = succ->left;
        }
    } else {
        for (succ = node->parent; node == succ->right ; succ = succ->parent) {
            node = succ;
        }
        if (succ == &tree->root) {
            succ = nil;
        }
    }

    return succ;
}

void
rbtrepair(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *parent;
    struct rbtnode *sibling;

    while (rbtgetcolor(node) == RBT_BLACK) {
        parent = node->parent;
        if (node == parent->left) {
            sibling = parent->right;
            if (rbtgetcolor(sibling) == RBT_RED) {
                rbtsetcolor(sibling, RBT_BLACK);
                rbtsetcolor(parent, RBT_RED);
                rbtrotleft(tree, parent);
                sibling = parent->right;
            }
            if (rbtgetcolor(sibling->right) == RBT_BLACK
                &&  rbtgetcolor(sibling->left) == RBT_BLACK) {
                rbtsetcolor(sibling, RBT_RED);
                node = parent;
            } else {
                if (rbtgetcolor(sibling->right) == RBT_BLACK) {
                    rbtsetcolor(sibling->left, RBT_BLACK);
                    rbtsetcolor(sibling, RBT_RED);
                    rbtrotright(tree, sibling);
                    sibling = parent->right;
                }
                rbtsetcolor(sibling, rbtgetcolor(parent));
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(sibling->right, RBT_BLACK);
                rbtrotleft(tree, parent);

                break;
            }
        } else {
            sibling = parent->left;
            if (rbtgetcolor(sibling) == RBT_RED) {
                rbtsetcolor(sibling, RBT_BLACK);
                rbtsetcolor(parent, RBT_RED);
                rbtrotright(tree, parent);
                sibling = parent->left;
            }
            if (rbtgetcolor(sibling->right) == RBT_BLACK
                && rbtgetcolor(sibling->left) == RBT_BLACK) {
                rbtsetcolor(sibling, RBT_RED);
                node = parent;
            } else {
                if (rbtgetcolor(sibling->left) == RBT_BLACK) {
                    rbtsetcolor(sibling->right, RBT_BLACK);
                    rbtsetcolor(sibling, RBT_RED);
                    rbtrotleft(tree, sibling);
                    sibling = parent->left;
                }
                rbtsetcolor(sibling, rbtgetcolor(parent));
                rbtsetcolor(parent, RBT_BLACK);
                rbtsetcolor(sibling->left, RBT_BLACK);
                rbtrotright(tree, parent);

                break;
            }
        }
    }

    return;
}

/* FIXME: this function is recursive */
void
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
_rbtdestroy(struct rbt *tree, struct rbtnode *node)
#else
_rbtdestroy(struct rbt *tree, struct rbtnode *node, void (*destroy)(void *))
#endif
{
    struct rbtnode *nil = &tree->nil;

    if (node != nil) {
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
        _rbtdestroy(tree, node->left);
        _rbtdestroy(tree, node->right);
#else
        _rbtdestroy(tree, node->left, destroy);
        _rbtdestroy(tree, node->right, destroy);
#endif
#if !defined(RBT_INTPTR_DATA) && !defined(RBT_UINTPTR_DATA)
        if (destroy) {
            destroy(node->data);
        }
#endif
        rbtputnode(node);
    }

    return;
}

void
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
rbtdestroy(struct rbt *tree)
#else
rbtdestroy(struct rbt *tree, void (*destroy)(void *))
#endif
{
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
    _rbtdestroy(tree, &tree->root);
#else
    _rbtdestroy(tree, &tree->root, destroy);
#endif
    free(tree);

    return;
}

#if defined(RBT_INTPTR_DATA)
intptr_t
#elif defined(RBT_INTPTR_DATA)
uintptr_t
#else
void *
#endif
rbtdelete(struct rbt *tree, struct rbtnode *node)
{
    struct rbtnode *nil = &tree->nil;
    struct rbtnode *node1;
    struct rbtnode *node2;
#if defined(RBT_INTPTR_DATA) || defined(RBT_UINTPTR_DATA)
    uintptr_t       data = node->data;
#else
    void           *data = node->data;
#endif

    if (node->left == nil || node->right == nil) {
        node2 = node;
    } else {
        node2 = rbtsuccessor(tree, node);
    }
    node1 = (node2->left == nil) ? node2->right : node2->left;
    node1->parent = node2->parent;
    if (node1->parent == &tree->root) {
        tree->root.left = node1;
    } else if (node2 == node2->parent->left) {
        node2->parent->left = node1;
    } else {
        node2->parent->right = node1;
    }
    if (rbtgetcolor(node2) == RBT_BLACK) {
        rbtrepair(tree, node1);
    }
    if (node2 != node) {
        node2->parent = node->parent;
        node2->left = node->left;
        node2->right = node->right;
#if !defined(RBT_DATA_COLOR)
        node2->color = node->color;
#endif
        node->left->parent = node2;
        node->right->parent = node2;
        if (node == node->parent->left) {
            node->parent->left = node2;
        } else {
            node->parent->right = node2;
        }
    }
    rbtputnode(node);

    return data;
}

