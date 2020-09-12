/*
 * THANKS
 * ------
 * - Henry 'froggey' Harrington for amd64-fixes
 * - Jester01 and fizzie from ##c on Freenode
 */

#ifndef __BSP_X86_64_SETJMP_H__
#define __BSP_X86_64_SETJMP_H__

#include <features.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>
#include <zero/cdefs.h>

struct _jmpbuf {
    int64_t  rbx;
    int64_t  r12;
    int64_t  r13;
    int64_t  r14;
    int64_t  r15;
    int64_t  rbp;
    int64_t  rsp;
    int64_t  rip;
#if defined(_POSIX_SOURCE)
    sigset_t sigmask;
#elif defined(_BSD_SOURCE)
    int      sigmask;
#endif
#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)
    long     havesigs;
#endif
};

typedef struct _jmpbuf jmp_buf[1];

struct _jmpframe {
    int64_t rbp;
    int64_t rip;
    uint8_t args[VLA];
};

/*
 * callee-save registers: rbp, rbx, r12...r15
 */
#define m_setjmp(env)                                                   \
    do {                                                                \
        __asm__ __volatile__ ("movq %0, %%rax\n"                        \
                              "movq %%rbx, %c1(%%rax)\n"                \
                              "movq %%r12, %c2(%%rax)\n"                \
                              "movq %%r13, %c3(%%rax)\n"                \
                              "movq %%r14, %c4(%%rax)\n"                \
                              "movq %%r15, %c5(%%rax)\n"                \
                              "movq %c6(%%rbp), %%rdx\n"                \
                              "movq %%rdx, %c7(%%rax)\n"                \
                              "movq %c8(%%rbp), %%rcx\n"                \
                              "movq %%rcx, %c9(%%rax)\n"                \
                              "leaq %c10(%%rbp), %%rdx\n"               \
                              "movq %%rdx, %c11(%%rax)\n"               \
                              :                                         \
                              : "m" (env),                              \
                                "i" (offsetof(struct _jmpbuf, rbx)),    \
                                "i" (offsetof(struct _jmpbuf, r12)),    \
                                "i" (offsetof(struct _jmpbuf, r13)),    \
                                "i" (offsetof(struct _jmpbuf, r14)),    \
                                "i" (offsetof(struct _jmpbuf, r15)),    \
                                "i" (offsetof(struct _jmpframe, rbp)),  \
                                "i" (offsetof(struct _jmpbuf, rbp)),    \
                                "i" (offsetof(struct _jmpframe, rip)),  \
                                "i" (offsetof(struct _jmpbuf, rip)),    \
                                "i" (offsetof(struct _jmpframe, args)), \
                                "i" (offsetof(struct _jmpbuf, rsp))     \
                              : "rax", "rcx", "rdx");                   \
    } while (0)

#define m_longjmp(env, val)                                             \
    do {                                                                \
        __asm__ __volatile__ ("movq %0, %%rcx\n"                        \
                              "movq %1, %%rax\n"                        \
                              "cmpq $0, %%rax\n"                        \
                              "jne 0f\n"                                \
                              "movq $1, %%rax\n"                        \
                              "0:\n"                                    \
                              "movq %c2(%%rcx), %%rbx\n"                \
                              "movq %c3(%%rcx), %%r12\n"                \
                              "movq %c4(%%rcx), %%r13\n"                \
                              "movq %c5(%%rcx), %%r14\n"                \
                              "movq %c6(%%rcx), %%r15\n"                \
                              "movq %c7(%%rcx), %%rbp\n"                \
                              "movq %c8(%%rcx), %%rsp\n"                \
                              "movq %c9(%%rcx), %%rdx\n"                \
                              "jmpq *%%rdx\n"                           \
                              :                                         \
                              : "m" (env),                              \
                                "m" (val),                              \
                                "i" (offsetof(struct _jmpbuf, rbx)),    \
                                "i" (offsetof(struct _jmpbuf, r12)),    \
                                "i" (offsetof(struct _jmpbuf, r13)),    \
                                "i" (offsetof(struct _jmpbuf, r14)),    \
                                "i" (offsetof(struct _jmpbuf, r15)),    \
                                "i" (offsetof(struct _jmpbuf, rbp)),    \
                                "i" (offsetof(struct _jmpbuf, rsp)),    \
                                "i" (offsetof(struct _jmpbuf, rip))     \
                              : "rax", "rbx", "rcx", "rdx",             \
                                "r12", "r13", "r14", "r15",             \
                                "rsp");                                 \
    } while (0)

#endif /* __BSP_X86_64_SETJMP_H__ */

