#ifndef __UCONTEXT_H__
#define __UCONTEXT_H__

#if !defined(__zen__)

#include <signal.h>
#include <bits/ucontext.h>

int  getcontext(ucontext_t *uc);
int  setcontext(const ucontext_t *uc);
void makecontext(ucontext_t *uc, void (*func)(void), int argc, ...);
int  swapcontext(ucontext_t *restrict olduc, const ucontext_t *restrict uc);

#endif /* !defined(__zen__) */

#endif /* __UCONTEXT_H__ */

