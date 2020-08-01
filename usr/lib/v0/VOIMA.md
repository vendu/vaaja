# Voima Game Console

Copyright (C) 2020 Tuomo Petteri Venäläinen. All rights reserved.

Voima is my on-going attempt to design and implement a modern microprocessor
based quite-heavily on RISC-machines of the past as well as some extra spice for
the purpose of multithreading, code optimization, and other tasks.

## Voima Instruction Set

The instruction set here is a simplified version of my earlier Velho
documentation. Instruction parces are aligned 32-bit words with slightly-
different syntax for main and coprocessor units. For the sake of simplicity,
I'll represent our instruction formats in C-syntax.

### Instruction Format

The NOP instruction is implemented as all 32 bits set to 1.

### Main Processor Units

struct v0ins {
    int8_t      op1     : 4;
    int8_t      op2     : 4;
    int8_t      reg1    : 4;
    int8_t      reg2    : 4;
    int8_t      reg3    : 4;
    int8_t      flg     : 4;
    int8_t      imm8;
};

### Coprocessor Units

struct v0copins {
    int8_t      unit    : 4;
    int8_t      op1     : 4;
    int8_t      op2     : 4;
    int8_t      reg1    : 4;
    int8_t      reg2    : 4;
    int8_t      reg3    : 4;
    int8_t      flg;
};

### Address Modes

*%r/(%r)        - indirect
$4(%r)          - indirect indexed
label           - direct

### Operand Brief

r               - register
v               - imm8-field in opcode
i               - [32-bit] immediate field following opcode
a               - memory address

### Logic/Control-Unit

#### 0x00 NOT   ri1, r2
#### 0x01 NEG   ri1, r2
#### 0x02 AND   ri1, r2
#### 0x03 OR    ri1, r2
#### 0x04 XOR   ri1, r2
#### 0x05 NAND  ri1, r2
#### 0x06 NOR   ri1, r2
#### 0x07 XNOR  ri1, r2

#### values 0x08..0x0f

Reserved for coprocessor identification

#### 0x08 FPU
#### 0x09 VEC
#### 0x0a DSP
#### 0x0b GPU
#### 0x0c FXP
#### 0x0d IOP
#### 0x0e BSP

### Arithmetic-Unit

#### flg-field

MSW-bit       - set processor flags CF, OF
UNS-bit       - execute instruction on unsigned operands
VAL-bit       - imm8-field effective
IMM-bit       - immediate operand present

#### 0x00 SRL   rv1, r2
#### 0x01 SLL   rv1, r2
#### 0x02 SAR   rv1, r2
#### 0x03 ADD   ri1, r2
#### 0x04 SUB   ri1, r2
#### 0x05 MUL   ri1, r2
#### 0x06 IRP   ri1, r2
#### 0x07 DIV   ri1, r2
#### 0x08 BSF   ri1, r2
#### 0x09 BSR   ri1, r2
#### 0x0a HAM   ri1, r2
#### 0x0b CMP   ri1, r2
#### 0x0c BEX   ri1, r2
#### 0x0d HWX   ri1, r2
#### 0x0e VBX   ri1, v2
#### 0x0e VHX   ri1, v2
#### 0x0f VWX   ri1, v2

### Flow-Unit

#### flg-field for BRA

ZF-bit
CF-bit
OF-bit
LT-bit

#### 0x00 JMP   ri1
#### 0x01 BRA   ri1
#### 0x02 CSR   ri1
#### 0x03 RET   ri1
#### 0x04 SME   ri1
#### 0x05 SMX   ri1
#### 0x06 THR   ri1, r2, r3     thr(adr, arg, retp); // retp goes in LR
#### 0x07 THX   ri1
#### 0x08 TSLP
#### 0x09 TWAIT ri1
#### 0x09 TJOIN ri1
#### 0x0a TDET  ri1

### Data-Unit

#### flg-field for load and store operations

MEM-bit      - one operand is an address
NDX-bit      - [indirect] indexed address; no BIT for indirect
DIR-bit      - direct address (with IMM, i.e. immediate value)
IMM-bit      - immediate operand present

#### 0x00 LDR   ari1, r2        - load register
#### 0x01 STR   ri1, ar2        - store register
#### 0x02 PSH   ri1             - push register or immediate
#### 0x03 POP   r1              - pop register
#### 0x04 STM                   - store many registers (bitmap)
#### 0x05 LDM                   - load many registers (bitmap)
#### 0x06 CLD   ari1, r2        - conditional register load
#### 0x07 CST   ri1, ar2        - conditional register store
#### 0x08 LNT   a1, r2          - nontemporal register load (bypass cache)
#### 0x09 SNT   r1, a2          - nontemporal register store
#### 0x0a LDX   ari1, s2        - load system register
#### 0x0b STX   s1, ar2         - store system register
#### 0x0c CPF   a1              - cacheline prefetch
#### 0x0d CAS   a1, r2, r3      - compare -and-swap
#### 0x0e BTS   a1, ri2, r3     - bit test-and-set
#### 0x0f BTC   a1, ri2, 3      - bit test-and-clear

### IO-Unit

#### 0x00 IOP   v1, i2          - permission control; SET, CLR, CHK (i2 == port)
#### oxo1 IOR   r1, i2
#### oxo1 IOW   r1, i2

## Register File

### General-Purpose Registers

General-purpose registers are 32-bit wide.

#### R1
#### R2
#### R3
#### R4
#### R5
#### R6
#### R7
#### R8
#### R9
#### R10
#### R11
#### R12
#### PC/R13
#### FP/R14
#### SP/R15
#### LR/R16

### System Registers

System registers are 32-bit wide.

#### MSW
#### TMR        - timer control; if (TMS & TMR == TMR) raise(TMR_TRAP);
#### TMS        - timestamp register
#### PDR        - page-directory register
#### IOV        - IOP-bit for per-port I/O-permission control
#### IMR        - interrupt mask register; 1-bit means masked
¤¤¤¤ TLS        - thread-local storage; page-address + page-count in low bits
