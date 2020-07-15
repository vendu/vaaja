#include <v0/vm/vm.h>

/*
 * callee-save structure
 *
 * struct v0calleectx {
 *     // v0word varsize; (local variables in bytes) goes here
 *     // local variables go here
 *     v0word      creg[V0_SAVE_REGISTERS];
 *     v0word      fp;          // caller frame pointer         <- FP after BEG
 *     v0word      lr;          // caller return address        <- SP after CSR
 *     v0word      args[VLA];   // stack argyments for callee   <- SP after CSR
 * };
 */

/*
 *
 * - push caller frame-pointer
 * - copy stack-pointer to frame-pointer
 * - save callee-save registers r4..r11
 * - allocate room for global variables by adjusting stack pointer
 *
 * stack layout after BEG
 *
 * ------
 * vars         caller local variables  <- caller stack-pointer
 * ------
 * parm
 * ------
 * lr           caller link-register    <- SP
 * ------
 * fp           caller frame-pointer
 * ------
 * r12/sp       callee-save registers
 * ------
 * ...
 * ------
 * r4                                   <- FP
 * ------
 * pad
 * ------
 * vars         local variables         <- SP
 * ------
 */
static __inline__ void
v0beg(struct vm *vm, v0word nb)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     sp = regs[V0_SP_REG];
    v0uword     fp = regs[V0_FP_REG];
    v0uword     pc = regs[V0_PC_REG];
    v0uword    *stk;

    pc += sizeof(struct v0ins);
    (v0uword *)&vm->mem[sp] = fp;
    sp -= V0_SAVE_REGISTERS * sizeof(v0uword);
    stk = (v0uword *)&vm->mem[sp];
    regs[V0_FP_REG] = sp;
    stk[0] = regs[V0_R4_REG];
    stk[1] = regs[V0_R5_REG];
    stk[2] = regs[V0_R6_REG];
    stk[3] = regs[V0_R7_REG];
    sp -= nb;
    stk[4] = regs[V0_R8_REG];
    stk[5] = regs[V0_R9_REG];
    stk[9] = regs[V0_R10_REG];
    sp = v0alnstk(sp, sizeof(v0wide));
    stk[7] = regs[V0_R11_REG];
    stk[8] = regs[V0_SP_REG];
    stk[9] = fp;
    regs[V0_SP_REG] = sp;

    return;
}

/*
 * - restore callee-save registers r4..r12 (r12 is stack-pointer)
 * - restore caller frame-pointer
 * - stack pointer is left pointing at stored caller frame-pointer
 */
static __inline__ void
v0fin(struct vm *vm, v0word nb)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     fp = regs[V0_FP_REG];
    v0uword     pc = regs[V0_PC_REG];
    v0uword    *stk = (v0uword *)&vm->mem[fp];

    regs[V0_R4_REG] = stk[0];
    regs[V0_R5_REG] = stk[1];
    regs[V0_R6_REG] = stk[2];
    regs[V0_R7_REG] = stk[3];
    regs[V0_R8_REG] = stk[4];
    fp += V0_SAVE_REGISTERS - 1;
    regs[V0_R9_REG] = stk[5];
    regs[V0_R10_REG] = stk[6];
    regs[V0_R11_REG] = stk[7];
    regs[V0_SP_REG] = stk[8];

    return;
}

/*
 * stack layout after CSR
 *
 * ------
 * vars         caller local variables  <- FP
 * ------
 * parm         stack arguments
 * ------
 * lr           caller link register    <- SP
 * ------
 *
 */
static __inline__ void
v0csr(struct vm *vm, v0uword adr)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     sp = regs[V0_SP_REG];
    v0uword     lr = regs[V0_LR_REG];
    v0uword     pc = regs[V0_PC_REG];

    sp -= sizeof(v0uword);
    pc += sizeof(struct v0ins);
    *(v0uword *)&g_vm.mem[sp] = lr;
    regs[V0_SP_REG] = sp;
    regs[V0_LR_REG] = pc;
    regs[V0_PC_REG] = adr;

    return;
}

/*
 * - restore caller link-register from stack
 * - copy link-register to program-counter
 *
 * stack layout before RET
 *
 * ------
 * vars         caller local variables
 * ------
 * parm
 * ------
 * lr                                   <- SP
 */
static __inline__ void
v0ret(struct vm *vm)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     fp = regs[V0_FP_REG];
    v0uword    *stk = (v0reg *)&g_vm.mem[fp];
    v0uword     pc;
    v0uword     lr;
    v0uword     sp;

    pc = *stk;
    fp += sizeof(v0uword);
    sp = stk[-1];
    regs[V0_SP_REG] = fp;
    regs[V0_LR_REG] = lr;
    regs[V0_PC_REG] = pc;

    return;
}

/*
 * struct v0sysctx {
 *     v0uword  msw;
 *     v0uword  ufp;
 *     v0uword  usp;
 *     v0uword  lr;
 * };
 *
 * system call arguments are in r0..r3
 *
 * stack after SYS
 *
 * ------
 * pc           user-mode return address
 * ------
 * usp          user-mode frame-pointer
 * ------
 * ufp          user-mode stack-pointer
 * ------
 * msw          machine status word     <- FP
 * ------
 */
static __inline__ void
v0sys(struct vm *vm, v0uword adr)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     sp = t_thr->sysregs[V0_SP0_REG];
    v0uword     pc = regs[V0_PC_REG];
    v0uword     usp = regs[V0_SP_REG];
    v0uword     ufp = regs[V0_FP_REG];
    v0uword     msw = t_thr->sysregs[V0_MSW_REG];
    v0uword    *stk;

    sp--;
    pc += sizeof(struct v0ins);
    stk = (v0uword *)&g_vm.mem[sp];
    sp -= 3 * sizeof(v0uword);
    *stk = pc;
    stk[-1] = usp;
    stk[-2] = ufp;
    stk[-3] = msw;
    t_thr->sysregs[V0_SP0_REG] = sp;
    t_thr->genregs[V0_SP_REG] = sp;
    t_thr->genregs[V0_FP_REG] = sp;
    t_thr->genregs[V0_PC_REG] = adr;

    return;
}

static __inline__ void
v0srt(struct vm *vm)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     sp = regs[V0_FP_REG];
    v0uword    *stk = (v0uword *)&vm->mem[sp];
    v0uword     msw = *stk;
    v0uword     ufp = stk[1];
    v0uword     usp = stk[2];
    v0uword     pc = stk[3];

    sp += 4 * sizeof(v0uword);
    t_thr->sysregs[V0_MSW_REG] = msw;
    regs[V0_SP_REG] = usp;
    regs[V0_FP_REG] = ufp;
    t_thr->sysregs[V0_SP0_REG] = sp;
    regs[V0_PC_REG] = pc;
}

/*
 * stack before IRT (64-bit aligned)
 *
 * ------
 * lr
 * ------
 * usp
 * ------
 * ufp
 * ------
 * msw                                  <- FP
 * ------
 * r15
 * ------
 * ...
 * ------
 * r0
 * ------
 */
static __inline__ void
v0irt(struct vm *vm)
{
    v0wide     *regs = &t_thr->genregs;
    v0uword     fp = regs[V0_FP_REG];
    v0uword    *stk = (v0uword *)&g_vm.mem[fp];
    v0wide     *ctx = (v0wide *)stk - V0_MAX_REGISTERS;
    v0uword     msw = *stk;
    v0uword     ufp = stk[1];
    v0uword     usp = stk[2];
    v0uword     pc = stk[3];
    v0uword     sp = fp + 4 * sizeof(v0uword);

    v0bcopyw(ctx, regs, V0_MAX_REGISTERS * sizeof(v0wide));
    regs[V0_SP_REG] = usp;
    regs[V0_FP_REG] = ufp;
    v0->sysregs[V0_SP0_REG] = sp;
    regs[V0_PC_REG] = pc;

    return;
}

#if defined(V0_PTHREAD)

/*
 * - launch new thread
 * - return thread ID
 */
static __inline__ long
v0thr(struct vm *vm, void *(*func(void *)), void *arg)
{
    struct v0thr       *thr = &t_thr;
    pthread_t          *tid = &t_thrid;

    v0initthr(thr);
    pthread_create(tid, NULL, func, arg);

    return;
}

static __inline__ v0word
v0thx(struct vm *vm, v0uword val)
{
    v0word *retptr = &t_thr.exitval;

    *retptr = val;

    return;
}

static __inline__ void
v0thw(struct vm *vm, pthread_t tid)
{
    pthread_join(tid, &t_exitval);

    return;
}

#endif

