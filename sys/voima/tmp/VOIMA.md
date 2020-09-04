# V0MI; V0 Minimal Instruction Set

The purpose of this minimalism is to cut down features not necessary [for a game
console]. Technically, it could run in single-thread mode but [static-priority]
thread scheduling makes programming the console easier and less error-prone. :)

Notice that we don't use system-mode in the console, nor virtual memory. Fun!

One thing worth trying would be variable-size instructions to reduce the size of
the game memory footprints.

## ALU

The ALU-instructions do NOT support immediate operands for the sake of code
size; use LDI or LDR to load [32-bit] values into the registers explicitly.

There is no third register argument; instead, most instructions overwrite the
destination register.

- MSW-bit                       set flag-bits in the MSW-register
- UNS-bit                       perform unsigned operation
- IMM32-bit                     immediate 32-bit operand following opcode

### LEA Addressing Modes
- REG-bit                       base address
- OFS-bit                       immediate 16-bit offset in opcode
- NDX-bit                       immediate 32-bit offset/address following opcode

$65536                          absolute
label                           direct
*%r1 or (%r1)                   register
$ofs(%r1)                       indexed indirect

struct vmimemins {
    int8_t  op  : 8;
    int8_t  reg : 4;
    int8_t  flg : 4;            // instruction flags
    int16_t ofs;                // offset relative to reg
    int32_t imm[];              // absolute or immediate relative to reg
};

### Stack Instructions
- SYS-bit                       // system register operands
- MAP-bit                       // bitmap of several register operands

struct v0mistkins {
    int8_t  op  : 8;            // PSH, POP, STM, LDM
    int8_t  reg : 4;            // destination register POP
    int8_t  flg : 4;            // instruction flags
    int16_t map;                // offset relative to stack pointer
};

### NOT r1, r2                  r2 = ~r1;      // invert all bits
### NEG r1, r2                  r2 = -r1;      // arithmetic negation
### AND r1, r2                  r2 &= r1;      // logical AND (equivalence)
### OR  r1, r2                  r2 |= r1;      // logical OR (set bits)
### XOR r1, r2                  r2 ^= r1;      // logical XOR (flip bits)
### SLL r1, r2                  r2 >>= r1;     // shift right, logical
### SRL r1, r2                  r2 <<= r1;     // shift left, logical
### SAR r1, r2                  r2 >>>= r1;    // shift right, arithmetic
### ADD r1, r2                  r2 += r1;      // addition
### SUB r1, r2                  r2 -= r1;      // subtraction
### MUL r1, r2                  r2 *= r1;      // multiplication
### DIV r1, r2                  r2 /= r1;      // c = irp(r1); r2 *= c;
### REM r1, r2                  r2 %= r1;      // c = irp(r1); r2 *= c;
### CMP r1, r2                  msw |= cmp(r1, r2); // flags as per r2 - r1
### BST r1, r2                  r2 | = r1;     // synonymomous with OR
### BCL r1, r2                  r2 &= ~r1;     // invert bits
### BFL r1, r2                  r2 ^= r1;      // synonymous with XOR

## FLO

### BRA   v1, ar2               branch conditionally or unconditionally (jump)
- if (!v1) { jmp(ar2); } else { bra(v1, ar2) };
- ZF-bit                        zero
- CF-bit                        carry
- OF-bit                        overflow
- LT-bit                        result less than

#### CSR    i16, ar2
##### call conventions
- the first 4 function arguments are in R1, R2, R3, and R4, respectively
- the rest of the arguments are in i16 aligned stack-words

- ALN-bit                       aligned by 16-bit padding (i.e. s2 comes first)
- i16 is offset to adjust stack with in the _callee_
- ar2 is subroutine address

- RET   ri1                     return from subroutine
  - ri1 is stack adjustment value to read original caller-registers

###### Pseudo-Code
- CSR_STM_BITS = R2|R3|R4|R5|R6|R7|R8|R9|R10|R11|PC|FP|SP|LR
	stm(CSR_STM_BITS);           32-bit instruction      0
	ldr(pc, lr);                 16-bit instruction      4       <- pc
	add(pc, 12);                 16-bit instruction      6
	ldr(sp, fp);                 16-bit instruction      8
	add(lr, 16);                 16-bit instruction      10      aret
	ldr(s1, r1);                 32-bit instruction      12
	csr(r1, ar2);                32-hit instruction      16
   - imm32                                              20
...                                                     24      <- lr
.align 32
	psh(fp);                     16-bit instruction      0
	ldr(sp, fp);                 16-bit instruction      2
	add(sp, nb);                 32-bit instruction      4

#### RET    r1
2. ldr(fp, sp);
3. pop(fp);
1. ldm(sp, R2|R3|R4|R5|R6|R7|R8|R9|R10|R11|PC|SP|LR);
5. jmp(lr);

#### THR   ar1                     launch new thread
.align 32
THR_DEF_ATR = THR_STK | THR_JOIN | THR_SYS_SCOPE | THR_SAME_SCHED
;; _kthr(m_adr_t atr, m_word_t flg);
_kthr:
                                ;;      size    offset  brief
                                ;;      ----    ------  -----
        ldr %r1, %sp            ;;      16-bit  0       sp = atr;
        ldr %sp, %r1            ;;      16-bit  2       r1 = atr.pret;
  	ldr sp, %r5             ;;      16-bit  4       r5 = atr;
  	ldr sp, %r6             ;;      16-bit  6       r6 = atr;
  	ldr sp, %r7             ;;      16-bit  12      r7 = atr;
  	add 4, %r5              ;;      16-bit  14      r5 = &atr.stk
  	add 8, %r6              ;;      16-bit  16      r6 = &atr.stksz;
  	add 12, %r7             ;;      16-bit  18      r7 = &atr.flg;
  	ldm *%r5, %sp           ;;      16-bit  20      sp = atr->stk;
  	ldx *%r6, %slim         ;;      16-bit  22      slim = atr->stksz;
  	ldm %r7, %r4            ;;      16-bit  24      r4 = atr->flg;
  	ldr %sp, %r2            ;;      16-bit  26      r2 = sp;
  	ldr %slim, %r3          ;;      16-bit  26      r3 = slim;
  	csr ksetsched           ;;      ksetsched(*pret, stk, slim, flg);
        
### THX ri1                     exit thread

.align 32
;; _kthx(m_word_t ret);
  1. dec(slim);
  2. lea(&k_thridmap, r2);
  2. ldr(slim, r3);
  3. add(r3, sp);
  4. thf(r2);
  5. pop(r3);
  5. ldr(r1, r3);
- TSLP                          put thread to sleep (to schedule another one in)
- WFE                           wait for memory event
                                - reduces busy-waiting/spinning for locks
- SEV                           signal memory event
## Instructions

### Pseudo-Code
- i16                           short (16-bit) immediate operand

#### STM    i16, r2             sp -= i16; stm(sp, r2);
