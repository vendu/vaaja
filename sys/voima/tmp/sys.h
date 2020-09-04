#ifndef __VOIMA_SYS_H__
#define __VOIMA_SYS_H__

/*
 * addressing modes; offsets may be 16- or 32-bit
 * - *%rx or (%rx)      // V0_ADR_REG_BIT
 * - $i                 // V0_ADR_IMM_BIT
 * - lbl                // V0_ADR_IMM_BIT
 * - $i(%rx)            // V0_ADR_REG_BIT && V0_ADR_IMM_BIT
 */

#define VOIMA_LEA       0x00    // load effective address
#define VOIMA_LDH       0x01    // load register with imm16 within opcode
#define VOIMA_LDI       0x02    // load register with imm32 after opcode
#define VOIMA_LDR       0x03    // load register-to-register
// r2 = rai1
#define VOIMA_STR       0x04    // store register
// *a2 &= ri1;
#define VOIMA_LDM       0x05    // load many registers
// load registers listed in imm16-bitmap
#define VOIMA_STM       0x06    // store many registers
// store registers listed in imm16-bitmap
#define VOIMA_LDX       0x07    // load system register
#define VOIMA_STX       0x08    // store system register
#define VOIMA_CLD       0x09    // conditional load depdning on flg-member bits
#define VOIMA_CST       0x0a    // conditional store depdning on flg-member bits
#define VOIMA_XCHG      0x0b    // atomic exchange
#define VOIMA_XINC      0x0c    // fetch-and-increment
// r2 = r1++;
#define VOIMA_XDEC      0x0d    // fetch-and-decrement
// r2 = r1--;
#define VOIMA_XADD      0x0e    // fetch-and-add
// r3 = r2; r2 += ri1;
#define VOIMA_CAS       0x0f   // compare-and-swap
// iff (*r1 == r2) { *r1 == r3; } r3 == r2; // *r1 == r3 -> success
#define VOIMA_JMP       0x10    // branch unconditionally
#define VOIMA_BRA       0x11    // branch according to ZF, LT, CF, OF in imm16
#define VOIMA_CSR       0x12    // call subroutine
/*
 * CALL
 * ----
 * - the caller stores R4..R1 on stack
 * - the caller pushes nstk-arguments on stack
 * - the caller loads R1..R4 with the first 4 arguments for the call
 * - the callee pushes LR (link-register/return address)
 * - the cellee does psh %fp; ldr %sp, %fp
 * - the callee stores, below the stack pointer, R5..R11 (scratch registers)
 * - the callee allocates storage for automatic variables on its stack; these
 *   will be accessed with negative offsets to the stack pointer
 */
#define VOIMA_RET       0x13    // return from subroutine
#define VOIMA_SME       0x14    // sme(ctx, func, flg);
#define VOIMA_SMX       0x15    // smx(ret);
#define VOIMA_THR       0x16    // thr(func, flg, *thr); // thr->id, thr->ret;
#define VOIMA_THX       0x17    // thx(thr, flg, ret);
#define VOIMA_CPF       0x18    // cpf(cl);
#define VOIMA_FCL       0x19    // flush cache
#define VOIMA_FPG       0x1a    // invalidate page-TLB entries
#define VOIMA_WFE       0x1b    // wait for [memory] event
#define VOIMA_SEV       0x1c    // signal event
#define VOIMA_INT       0x1d    // send software interrupt/signal
#define VOIMA_ION       0x1e    // enable interrupts
#define VOIMA_IOF       0x1f    // disable interrupts

#endif /* __VOIMA_SYS_H__ */

