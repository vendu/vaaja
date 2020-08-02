# Voima Game Console

Copyright (C) 2020 Tuomo Petteri Venäläinen. All rights reserved.

Voima is my on-going attempt to design and implement a modern microprocessor
based quite-heavily on RISC-machines of the past as well as some extra spice for
the purpose of multithreading, code optimization, and other tasks.

## Voima Instruction Set

The instruction set here is a simplified version of my earlier Velho
documentation. Instruction parcels are aligned 32-bit words with slightly-
different syntax for main and coprocessor units.

## Instruction Format

The NOP instruction is implemented as all 32 bits set to 1.

For the sake of simplicity, I'll represent our instruction formats in C-syntax
structures suitable for use in virtual machines.

#define V0_INS_COP_BIT          0x80
struct v0ins {
    int8_t      uop;            // instruction ID; dispatch if highest bit set
    int8_t      reg1    : 4;    // register operand #1
    int8_t      reg2    : 4;    // register operand #2
    int8_t      reg3    : 4;    // register operand #3
    int8_t      flg     : 4;    // operation flags
    int8_t      imm8;           // optional 8-bit immediate value
    m_word_t    imm[];          // [aligned] word-operand (or address)
};

#define v0alnimm64(cop) roundup2(&cop->end, 8)
struct v0copins {
    int8_t      id      : 3;    // coprocessor ID
    int8_t      op      : 5;    // instruction ID
    int8_t      reg1    : 4;    // register operand #1
    int8_t      reg2    : 4;    // register operand #2
    int8_t      flg;            // instruction flags
    int8_t      imm8;           // optional 8-bit immediate value
    int32_t     end32[];        // align if more than 32-bit of alignment needed
};

## Address Modes

- *%r/(%r)                      indirect
- $4(%r)                        indirect indexed
- label                         direct

## Operand Brief

- r                             register
- v                             imm8-field in opcode
- i                             [32-bit] immediate value following opcode
- a                             memory address

### Standard Coprocessor IDs

#### 0x00 CPU                   main processor unit(s)
#### 0x01 FPU                   floating-point coprocessor
#### 0x02 VEC                   vector/SIMD coprocessor
#### 0x03 DSP                   digital signal coprocessor
#### 0x04 GPU                   graphics coprocessor
#### 0x05 FXP                   fixed-point coprocessor
#### 0x06 BSP                   board support package (firmware etc.)
#### 0x07 AUX                   supplementary coprocessor

### ALU; Arithmetic-Logical Unit

#### flg-field
- MSW-bit                       set processor flags CF, OF
- UNS-bit                       execute instruction on unsigned operands
- VAL-bit                       imm8-field effective
- IMM-bit                       immediate operand present

#### 0x00 NOT   ri1, r2         r2 = ~ri1;
#### 0x01 NEG   ri1, r2         r2 = -ri1;
#### 0x02 AND   ri1, r2, r3     r3 = r1 & r2;
#### 0x03 OR    ri1, r2, r3     r3 = r1 | r2;
#### 0x04 XOR   ri1, r2, r3     r3 = r1 ^ r2;
#### 0x05 NAND  ri1, r2, r3     r3 = r1 ~& r2;
#### 0x06 NOR   ri1, r2, r3     r3 = r1 ~| r2;
#### 0x07 XNOR  ri1, r2, r3     r3 = r1 ~^ r2;
#### 0x08 SRL   rv1, r2, r3     r3 = r2 >> rv1;
#### 0x09 SLL   rv1, r2, r3     r3 = r2 << rv1;
#### 0x0a SAR   rv1, r2, r3     r3 = r2 >>> rv1;
#### 0x0b ADD   ri1, r2, r3     r3 = r2 + ri1;
#### 0x0c SUB   ri1, r2, r3     r3 = r2 - ri1;
#### 0x0d MUL   ri1, r2, r3     r3 = r2 * ri1;
#### 0x0e MHI   ri1, r2, r3     r3 = (r2 * ri1) >> 32;
#### 0x0f IRP   ri1, r2         r2 = irp(ri1);          // inverse reciprocal
#### 0x10 DIV   ri1, r2, r3     r3 = r2 / ri1;          // caches reciprocal
#### 0x11 BSF   r1, r2          r2 = ctz(r1);           // count trailing zeroes
#### 0x12 BSR   r1, r2          r2 = clz(r1);           // count leading zeroes
#### 0x13 HAM   r1, r2          r2 = ham(r1);           // count bit-population
#### 0x14 CMP   ri1, r2         set MSW-bits ZF and LT for r2 - r1
#### 0x15 TST   ri1, r2, r3     r3 = r2 & ri1;          // extract bits
#### 0x16 BSWP  ri1, r2         r2 = bswap(ri1);        // swap byte-order
#### BST        synonymous with OR                      // set bits
#### BCL        synonymous with &= ~src;                // clear bits
#### BFL        synonymous with ^= src;                 // flip bits
- IDs 0x17..0x1f reserved for possible extensions

### FLO; Flow-Unit

#### 0x20 BRA   ri1             jmp(ri1);               // branch (see below)
- no bits set  - branch unconditionally
- ZF-bit       - zero
- CF-bit       - carry
- OF-bit       - overflow
- LT-bit       - less than
#### 0x21 CSR   ari1            call subroutine
#### 0x22 RET   ri1             return from subroutine  // stack adjustment
#### 0x23 SME   ari1            enter system mode       // documented below
#### 0x24 SMX   ri1             leave system mode       // documented below
#### 0x25 THR   ari1, r2, r3    thr(adr, atr, arg);     // psh(atr->pret);
#### 0x26 THX   ri1             thf(&thridmap); thx(ri1);
#### 0x27 TSLP  ri1             thrsleep(wc)             // sleep/wait
- wait channel is likely to be a system structure/data address
#### 0x28 TWAKE ri1             wake up thread(s) waiting on channel ri1
#### 0x29 TJOIN ri1             thrjoin(id);            // join thread
#### 0x2a TDET  ri1             thrdet(id);             // detach thread
#### 0x2b TLS   ari1, r2        load the TLS-register
- ari1 is an aligned page-address
- r2 is page count
#### 0x2c TWM   ari1            wait for memory event; operand is cacheline or
                                any if omitted
- if no bits set, default to RD | WR
- RD-bit                        wait for read event
- WR-bit                        wait for write event
- RM-bit                        read from main memory
- WB-bit                        write to main memory
#### 0x2d TWI   ri1             wait for I/O-event; operand is port or DMA-buf
- default to waiting on an I/O-port
- RD-bit                        wait for read event
- WR-bit                        wait for write event
- DMA-bit                       wait on DMA-buffer operation (media, network)
#### 0x2e TEV   (v1, ari2)      signal event
- v1 is one of MEMRD, MEMWR, MEMRM, MEMWB, IORD, IOWR, IODMA if present
- optional operand ari2 is cacheline address for event
#### WFE                        synonymous with TWM without optional operand
#### SEV                        synonymous with TEV without [optional] operands
####

### Data-Unit

#### flg-field for load and store operations

- MEM-bit    - one operand is an address
- NDX-bit    - [indirect] indexed address; no BIT for indirect
- DIR-bit    - direct address (with IMM, i.e. immediate value)
- IMM-bit    - immediate operand present

#### 0x30 LDR   ari1, r2        load register
#### 0x31 STR   ri1, ar2        store register
#### 0x32 PSH   ri1             push register or immediate
#### 0x33 POP   r1              pop register
#### 0x34 STM   ri1             store many registers (bitmap)
#### 0x35 LDM   ri1             load many registers (bitmap)
#### 0x36 CLD   ari1, r2        conditional register load
#### 0x37 CST   ri1, ar2        conditional register store
#### 0x38 LNT   a1, r2          nontemporal register load (bypass cache)
#### 0x39 SNT   r1, a2          nontemporal register store
#### 0x3a LDX   ari1, s2        load system register
#### 0x3b STX   s1, ar2         store system register
- IDS 0x3c..0x3f reserved

### ATOM; Atomic Operations Unit

#### flg-field
- PF-bit                        fetch old value before XINC, XDEC, XADD
- OK-bit                        set OK-bit in MSW on success
- NT-bit                        non-temporal cache fetch
- L1-bit                        prefetch into L1 and L2 (default is L2 only)

#### 0x40 CLK   a1              lock cacheline for current thread
#### 0x41 CPF   a1, (r2)        cacheline prefetch
- r2 is one of PFL1, PFL2, PFNT
  - PFL1 prefetches into L1 but not L2
  - PFL2 prefetches into L2 but not L1
#### 0x42 CFL   a1              force cacheline write-back to main memory
#### 0x43 BTS   a1, ri2, r3     bit test-and-set; may set OK-bit if bit was 0
#### 0x44 BTC   a1, ri2, 3      bit test-and-clear; may set OK-bit if bit was 1
#### 0x45 XINC  a1, (r2)        r2 = a1++; // don't fetch if PF-bit not set
#### 0x46 XDEC  a1, (r2)        r2 = a1--; // don't fetch if PF-bit not set
#### 0x47 XADD  ri1, a2, r3     r3 = a2; a2 += ri1; // don't fetch if PF not set
#### 0x45 CAS   a1, r2, r3      compare-and-swap; set OK-bit in MSW on success
- a1 is destination address
- r2 is the value we want there
- r3 is the replacement value

### IOP; IO-Unit

#### 0x50 IOP   v1, i2          permission control; SET, CLR, CHK (i2 == port)
#### 0x51 IOR   r1, i2          I/O-port read operation
- BYTE-bit                      byte (8-bit)
- HALF-bit                      halfword (16-bit)
- WORD-bit                      word (32-bit)
#### 0x52 IOW   r1, i2          I/O-port write operation
#### 0x53 IOC   v, ri1, ar2, r3 I/O-commands
- v                             system/hardware device ID
- ri1                           DEVINIT, DEVSEEK, DMASET, DMARD, DMAWR, ...
- ar2                           source or destination address
- r3                            status; 0 for timeout, positive for count,
                                negative for error (EDEV, EPERM, ESYS)
  - EDEV                        device I/O-failure
  - EPERM                       inadequeate I/O-permissions
  - ESYS                        system-defined error code
- ioc(bus, dev, atr);           atr is initialised to operation attributes,
                                returns status attributes
- IDs 0x54..0x5f reserved

##### TODO

struct dmaop {
    m_word_t        op;
    pid_t           pid;        checked against THR-register
    struct dmabuf  *buf;
};

struct dmabuf {
    off_t           ofs;
    adr_t           phys;
    pid_t           pid;
    struct buf     *buf;
};

### SYS; Sys-Unit               System Control Unit

#### 0x60 HLT ri1               wait for unmasked interrupts (any if ri1 == 0)
- ri1 is interrupt-mask with 1-bits for masked [user] interrupts
- system interrupts 0x00..0x1f cannot be masked
#### 0x61 RST ri1               reset
- ri1 is command                COLD, WARM, HIBER, HALT
#### 0x62 STF ri1               MSW |= ri1;
#### 0x63 CLF ri1               MSW &= ~ri1;
#### 0x64 PSHF                  --*%sp = MSW;
#### 0x65 POPF                  MSW = *%sp++;
#### 0x66 CINV                  flush internal caches
#### 0x67 PINV  a1              invalidate TLB-entry for page a1
#### 0x68 CPF   a1              prefetch cacheline containing a1
#### 0x69 CLK   a1              lock cacheline containing a1 for current thread
#### 0x6a THF   a1              ldr(tls, r2); sll(r3, r2); *a1 |= r3; 
- IDs 0x6a..0x6f reserved

### Register File

#### General-Purpose Registers

General-purpose registers are 32-bit wide.

##### R0                        zero-register; reads return zero, writes ignored
- may be used to represent missing register operand
##### R1
##### R2
##### R3
##### R4
##### R5
##### R6
##### R7
##### R8
##### R9
##### R10
##### R11
##### PC/R12
##### FP/R13
##### SP/R14
##### LR/R15

#### System Registers

System registers are 32-bit wide.

##### MSW/R0
- ZF-bit                        zero results
- CF-bit                        carry/borrow-bit
- OF-bit                        overflow-bit
- LT-bit                        comparison resulted in less than
- AC-bit                        alignment check failure
- MC-bit                        machine check failure
- BC-bit                        bound-checking in action
- OK-bit                        operation (e.g. atomic one) successful
##### TMR/R1                    timer control; TMS & TMR == TMR -> #TMR_TRAP;
##### TMS/R2                    timestamp register
##### IVR/R3                    interrupt vector; vector-address + flags
##### PDR/R4                    page-directory register
##### IOR/R5                    port-permission bitmap or 0 if not utilised
##### IMR/R6                    interrupt mask register; 1-bit means masked
##### THR/R7                    ID of currently running thread
##### TRP/R8                    thread return pointer
##### SLIM/R9                   [bottom] limit of thread stack
##### TLS/R10                   thread-local storage
- high 20 bits contain page-address
- low 12 bits store page-count for the segment
##### BLO/R11                   low [array] bound/limit
##### BHI/R12                   high [array] bound/limit

### FPU; Floating-Point Coprocessor

#### 0x00 FLS                   load FSR-register
#### 0x00 FSS                   store FSR-register
#### 0x03 FSC                   store FPU-context/state (all registers)
#### 0x04 FLD                   load value into floating-point register
- MEM-bit                       source is memory address
- FLT-bit                       load single-precision value
#### 0x04 FST
- MEM-bit       - destination is memory address
- FLT-bit       - store single-precision value
#### 0x05 FADD
#### 0x06 FSUB
#### 0x07 FMUL
#### 0x08 FIRP
#### 0x09 FDIV
#### 0x0a FSIN
#### 0x0b FCOS
#### 0x0c FTAN
#### 0x0d FASIN
#### 0x0e FASIN
#### 0x0f FATAN
#### 0x10 FSINH
#### 0x11 FCOSH
#### 0x12 FTANH
#### 0x13 FABS       - absolute value
#### 0x14 FCMP       - compare two floating-point values
#### 0x15 FSTI       - store floating-point value into integer register
- TF-bit               - truncate result
- TRZ-bit              - round towards zero
- TRI-bit              - round towards [positive] infinity
- TRNI-bit             - round towards negative infinity
#### 0x12 FTR        - truncate floating-point value (to integer)
#### 0x13 FCLD       - conditional floating-point load
- ZF-bit               - zero/equal-bit
- LT-bit               - less than
- FO-bit               - floating-point overflow
- FU-bit               - floating-point underflow
#### 0x14 FCST       - conditional floating-point store

#### 0x16 FLG2       - base-2 logarithm
#### 0x17 FLN        - natural logarithm
#### 0x18 FPOW       - exponentiation (raise to given power)
#### 0x19 FNORM      - normalize floating-point value
#### 0x1a FMA        - fused multiply-and-add

#### Floating-Point Coprocessor Registers

Floating-point registers are IEEE-format 64-bit.

##### F0..F14  - general purpose floating-point registers

##### FSR/F15  - floating-point status register
- default to performing operations on 80-bit extended precision values
  - 64-bit mantissa, 16-bit exponent
- FLT-bit       - operations are performed on 32-bit single-precision values
- DBL-bit       - operations are performed on 64-bit double-precision values
- FR0-bit       - round towards zero (default is round-to-nearest)
- FRP-bit       - round towards [positive] infinity
- FRNI-bit      - round towards negative infinity
- FDZ-bit       - division by zero
- FINX-bit      - inexact result
- FINV-bit      - invalid operand
- FOF-bit       - overflow
- FUF-bit       - underflow

### VEC; Vector Coprocessor

#### 0x00 BEX   - convert bytes to halfword or halfwords to word values.
#### 0x01 BPK   - pack word to halfword or halfword to byte values (SAT)
#### 0x02 VAND  - mask vector values
#### 0x03 VOR   - set vector-value bits
#### 0x04 VNAND - flip vector-value bits
#### 0x05 VSRL  - shift vector values to right logically
#### 0x06 VSLL  - shift vector values to left (logically)
#### 0x07 VSAR  - shift vector values to right arithmetically
#### 0x08 VADD  - add vectors
#### 0x09 VSUB  - subtract vectors
#### 0x0a VMUL  - multiply vectors
#### 0x0b VDIV  - divide vectors

#### Vector Coprocessor Registers

The vector coprocessor performs single-instruction-multiple-data (SIMD)
operations on vector subvalues.

Vector registers are 64-bit wide.

##### V0..V14    - general purpose vector registers
##### VSR/V15    - vector status register
- UNS-bit        - perform unsigned operations
- SAT-bit        - apply saturation (unsigned if UNS set, otherwise signed)
- HI-bit         - high-result words (e.g. pack-instructions)
- BYTE-bit       - byte (8 x 8-bit) vector mode
- HALF-bit       - halfword (4 x 16-bit) vector mode
- WORD-bit       - word (2 x 32-bit) vector mode

#### DSP; Digital Signal Processor

The digital signal registers are 32-bit IEEE single-precision floating-point
format.

### FXP; Fixed-Point Coprocessor

#### Fixed-Point Coprocessor Registers

Fixed-Point register are 64-bit wide; 32-bit integer + 32-bit fraction.

##### FX0..FX14  - general purpose fixed-point registers
##### FXSR       - fixed-point status register
- UNS           - perform unsigned operations
- FXC           - fixed-point carry-bit
- FXR           - round up (fraction >= 1/2)

# TODO

## cache memory

typedef uint32_t cltag;         // cacheline address, WRE-bit, WRD-bit
struct {
    int8_t data[NCLW];          // for 32-bit processors, NCLW == 32 (bytes)
};

## firmware
- ROM-filesystem
- system bootstrap

