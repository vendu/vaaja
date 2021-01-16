#ifndef __VPC_STK_H__
#define __VPC_STK_H__

#define VPC_STK_ITEMS   16

#define stkinit(stk)    ((stk)->top = (void *)&stk->top)
#define stkpush(stk, i) (*--((stk)->top) = (i))
struct stk {
    struct stkitem     *top;
    struct stkitem      tab[VPC_STK_ITEMS];
};

#endif __VPC_STK_H__

