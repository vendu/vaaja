# preface

voima is a project to design and implement a new game console. depending of the configuration, the console has the following hardware and software parts.

# voima console
- v0 (custom verilog source) or risc-v (bonfire) microprocessor
- zen operating system
  - kernel
  - libc
    - iso c standard library with unix-like functionality
  - libm
    - iso c mathematics library
  - sdl
    - [software directmedia layer](http://libsdl.org/)
  - libdeck
    - terminal interface
    - keyboard
    - pointer; mouse, joystick, pad, tab
    - scan
    - print
    - plot
  - libaud
  - libgfx
    - obj
      - get, put, free
    - pix
    - ipc
    - rop
    - conf
    - gfx; init, setup, run
    - draw; pnt, line, tri, rect, circ, ell, poly
    - img
    - color
    - sprite
    - fx
    - xform
    - shader

# voima instruction set

## instruction formats

### reg-format

- flg == 1 => atomic
  - NOT     %r1, %r2            ; r1 = ~r2;
  - LOR     %r1, %r2, %r3       ; r1 = r2 | r3;
  - XOR     %r1, %r2, %r3       ; r1 = r2 ^ r3;
  - AND     %r1, %r2, %r3       ; r1 = r2 & r3;
  - NOR     %r1, %r2, %r3       ; r1 = r2 ~| r3;
  - XNOR    %r1, %r2, %r3       ; r1 = r2 ^~ r3;
  - NAND    %r1, %r2, %r3       ; r1 = r2 ~& r3;
- flg == 1 => shift right
  - SHL     %r1, %r2, %r3|$i    ; r1 = r2 << %r3|$i;
  - SHR     %r1, %r2, %r3|$i    ; r1 = r2 >> %r3|$i;
  - SAR     %r1, %r2, %r3|$i    ; r1 = r2 >>> %r3|$i;
  - ROL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i;
  - ROR     %r1, %r2, %r3|$i    ; r1 = r2 -> %r3|$i;
  - RCL     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);
  - RCR     %r1, %r2, %r3|$i    ; r1 = r2 <- %r3|$i; setmsw(CF);
- INC, UINC, XINC, DEC, UDEC, XDEC              - flg == 1 => atomic
- ADD, UADD, ADC, XADD, SUB, USUB, SBB, XSUB    - flg == 1 => atomic
- MUL, UMUL, MHI, UMHI                          - flg == 1 => return high word
- DIV, UDIV, RDIV, URDIV                        - flg == 1 => compute remainder
- REM, UREM, IRP, UIRP
- BST, BCL, BFL
- BTS, BTC, BTF                                 - flg == 1 => atomic
- BFS, BLS, PAR, HAM
- CTZ, CLZ, HSH, HUN

fmt | 0..4   | 5..8 | 9..12 | 13..16 | 17..22 | 23  | 24..31 |
----|--------|------|-------|--------|--------|-----|--------|
reg | opcode | dst  | src1  | src2   | cnt    | flg | parm   |

### imm-format

- ZEX, SEX                                      - flg == 1 => sign-extend

fmt | 0..4   | 5..8 | 9..12 | 13..14 | 15  | 16..31 |
----|--------|------|-------|--------|-----|--------|
imm | opcode | dst  | src   | size   | flg | imm16  |

### jmp-format

- the effective jump address is { PC[31:28], adr, 00 }
- in addition, JAL stores PC + 8 to LR

fmt | 0..4   | 5..31 |
----|--------|-------|
jmp | opcode | adr   |

