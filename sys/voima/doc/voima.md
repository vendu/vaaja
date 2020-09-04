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

## notes

- the bits [0:11] (the low 12 bits) of the val-member are 6-bit operation ID
  + shift-count, bit-position, or some other 6-bit parameter
- the bits [0:11] (the low 12 bits) of the val-field may form a port ID for I/O
- the bits [12:15] (the high 4-bit nybble), i.e. the info-field, are operation
  info such as flags

# 0x00 - system

- MSW - ION, NOC, CWT
- TWC: cladr

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
NOP     |  0x00 |  0x00 | 0x00  |  0x00 | 0x00  | all 0-bits (32)
HLT     |  ri1  |  ---- | -X--  |  0x01 | ----  | RST/interrupt in ri1 resumes
CLI     |  ri1  |  ---- | -X-I  |  0x02 | ----  | X (register) or I -> mask
STI     |  ri1  |  ---- | -X-I  |  0x03 | ----  | X (register) or I -> mask
INT     |  rv1  |  ---- | V---  |  0x04 | ----  | raise interrupt rv1
WFE     |  ri1  |  ---- | ---I  |  0x05 | ----  | wait for event on cacheline
SEV     |  ri1  |  ---- | -X-I  |  0x06 | ----  | cacheline event; no ri1 -> all
CPF     |  ri1  |  ---- | ---I  |  0x07 | ----  | cacheline prefetch
SCP     |  rv1  |  ---- | V---  |  0x08 | ----  | set [global] cache policy
FLC     |  ri1  |  ---- | -X-I  |  0x09 | ----  | flush cacheline; no ri1 -> all
FPG     |  ri1  |  ---- | ---I  |  0x0a | ----  | flush page entry or all
MTR     |  ri1  |  r2   | VC--  |  0x0b | TCPB  | clear or set memory-type range

- MTR adr, size;        - T = write-through
                          C = write-combining
                          P = write-protect
                          B = write-back

# 0x01 - logic instructions
- parm: operation size in high 2 bits - shift-count for 1 [byte]; MEM, LKB
- val: operand ID

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
NOT     |  rv1  |  ---  | V---  |  0x01 | ----  | logical NOT (single-operand)
OR      |  rv1  |  ra2  | V---  |  0x02 | ANO-  | OR, dual-operand, atomic
AND     |  rv1  |  ra2  | V---  |  0x03 | ANO-  | AND, dual-operand, atomic
XOR     |  rv1  |  ra2  | V---  |  0x04 | ANO-  | XOR, dual-operand, atomic
NOR     |  rv1  |  r2   | V---  |  0x05 | ----  | NOR, dual-operand
XNOR    |  rv1  |  r2   | V---  |  0x06 | ----  | XNOR, dual-operand
NAND    |  rv1  |  r2   | V---  |  0x07 | ----  | NAND, dual-operand
-----------------------------------------------

+ NOT   rdst;
+ OR    rsrc, r/adst; (MEM|LKB)
+ AND   rsrc, r/adst; (MEM|LKB)
+ XOR   rsrc, r/adst; (MEM|LKB)
+ NOR   rsrc, rdst
+ XNOR  rsrc, rdst
+ NAND  rsrc, rdst

# 0x02 - shift instructions
- parm: CNT, CF, ARI, ROT
- val: (parm & CNT) -> val is immediate shift count

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
SLL     |  rv1  |  r2   | V---  |  0x00 | cnt   | shift-count, left
SRL     |  rv1  |  r2   | VD--  |  0x00 | cnt   | right
ROL     |  rv1  |  r2   | V-R-  |  0x00 | cnt   | shift-count, rotate left
ROR     |  rv1  |  r2   | VDR-  |  0x00 | cnt   | rotate right
RCL     |  rv1  |  r2   | V-R-  |  0x01 | cnt   | rotate left, sets CF/MSW
RCR     |  rv1  |  r2   | VDR-  |  0x01 | cnt   | rotate right, sets CF/MSW
SAR     |  rv1  |  r2   | VD--  |  0x02 | cnt   | shift-count, arithmetic
------------------------------------------------------

# 0x03 - arithmetic instructions
- parm: UNS, MSW, HIW, LKB

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
INC     |  ri1  |  r2   | ---I  |  0x00 | ----  | increment by one
UINC    |  ri1  |  r2   | U--I  |  0x00 | ----  | unsigned
XINC    |  ri1  |  a2   | UX-I  |  0x00 | ANO-  | unsigned, atomic
DEC     |  ri1  |  r2   | ---I  |  0x01 | ----  | decrement by one
UDEC    |  ri1  |  r2   | U--I  |  0x01 | ----  | unsigned
XDEC    |  ri1  |  a2   | UX-I  |  0x01 | ANO-  | unsigned, atomic
ADD     |  ri1  |  r2   | ---I  |  0x02 | ----  | addition
UADD    |  ri1  |  r2   | U--I  |  0x02 | ----  | unsigned, sets OF/MSW
XADD    |  ri1  |  a2   | UX-I  |  0x02 | ANO-  | unsigned, atomic
ADC     |  ri1  |  r2   | --FI  |  0x02 | ----  | sets CF/MSW
SUB     |  ri1  |  r2   | ---I  |  0x03 | ----  | subtraction
USUB    |  ri1  |  r2   | U--I  |  0x03 | ----  | unsigned
XSUB    |  ri1  |  a2   | UX-I  |  0x03 | ----  | unsigned, atomic
SBB     |  ri1  |  r2   | --FI  |  0x03 | ----  | sets CF/MSW
MUL     |  ri1  |  r2   | --FI  |  0x04 | ----  | multiplication, sets OF/MSW
UMUL    |  ri1  |  r2   | U-FI  |  0x04 | ----  | unsigned, sets OF/MSW
MHI     |  ri1  |  r2   | -X-I  |  0x04 | ----  | returns high result-word
UMHI    |  ri1  |  r2   | UX-I  |  0x04 | ----  | unsigned, returns high word
DIV     |  ri1  |  r2   | ---I  |  0x05 | ----  | division
UDIV    |  ri1  |  r2   | U--I  |  0x05 | ----  | unsigned
RDIV    |  ri1  |  r2   | -X-I  |  0x05 | r3    | returns remainder in r3
URDIV   |  ri1  |  r2   | UX-I  |  0x05 | r3    | unsigned
REM     |  ri1  |  r2   | ---I  |  0x06 | ----  | remainder
UREM    |  ri1  |  r2   | U--I  |  0x06 | ----  | unsigned
IRP     |  ri1  |  r2   | ---I  |  0x07 | ----  | inverse reciprocal
UIRP    |  ri1  |  r2   | U--I  |  0x07 | ----  | unsigned

# 0x04 - bit operations
- parm: RET, LKB, CNT
- parm: CLR, FLP
- val: (parm & CNT) -> val is immediate bit position for BST, BCL, BFL,
                       BTS, BTC, BTF
- val: function (for FUN)
- SET

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
BST     |  r1   |  ra2  | ----  |  0x00 | ANO-  | set bit, atomic
BCL     |  r1   |  ra2  | -C--  |  0x00 | ANO-  | clea bit, atomic
BFL     |  r1   |  ra2  | --F-  |  0x00 | ANO-  | flip bit, atomic
BTS     |  r1   |  ra2  | ----  |  0x01 | ANO-  | test-and-set bit, atomic
BTC     |  r1   |  ra2  | -C--  |  0x01 | ANO-  | test-and-clear bit, atomic
BTF     |  r1   |  ra2  | --F-  |  0x01 | ANO-  | test-and-flip bit, atomic
CTZ     |  r1   |  ra2  | L---  |  0x02 | ANO-  | count trailing (low) zero-bits
CLZ     |  r1   |  ra2  | ----  |  0x02 | ANO-  | count leading zero-bits
PAR     |  r1   |  ra2  | -OM-  |  0x02 | ANO-  | compute parity (0 for even)
HAM     |  r1   |  ra2  | -O--  |  0x03 | ANO-  | count one-bits
HSH     |  r1   |  ra2  | ----  |  0x03 | ANO-  | hash value
HUN     |  r1   |  ra2  | R---  |  0x03 | ANO-  | reverse hash to original value

# 0x05 - control flow

# 0x06 - memory
- parm: MEM, NT, IML

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
LEA     |  r1   |  r2   | -XLI  |0x00|r3| ANO-  | calculate address
LDI     |  i1   |  r2   | --LI  |  0x00 | ----  | load immediate
MVR     |  r1   |  r2   | ----  |  0x00 | ----  | register-to-register
LDR     |  a1   |  r2   | M---  |  0x00 | ANO-  | memory-to-register
LDL     |  a1   |  r2   | ----  |  0x00 | ANOL  | linked, memory-register
LDC     |  r1   |  r2   | M-C-  |  0x00 | ANO-  | conditional, memory-register
MVU     |  r1   |  r2   | ----  |  0x01 | ----  | system-to-user register
MVX     |  r1   |  r2   | -X--  |  0x01 | ----  | user-to-system register
MVC     |  r1   |  r2   | --C-  |  0x01 | ----  | conditional, register-register
STR     |  r1   |  a2   | ----  |  0x02 | ANO-  | register-to-memory
STX     |  r1   |  a2   | -X--  |  0x02 | ANO-  | system-register-to-memory
STC     |  r1   |  a2   | --C-  |  0x02 | ANO-  | conditional, register-memory
STL     |  r1   |  a2   | ----  |  0x02 | ANOL  | linked, register-memory
CAS     |  r1   |  r2   |  r3   |  0x03 | ANOL  | compare-and-swap, sets ZF/MSW

LDL adr, reg;
STL reg, adr;
CAS want, val, adr; set ZF on success

# 0x07 - I/O

inst    |  src  |  dst  | parm  |  val  | info  | notes
--------|-------|-------|-------|-------|-------|------
IOP     |  rv1  |  ri2  | V-CI  |  port | 0x00  | allow or deny I/O for port src
IOR     |  rv1  |  r2   | V---  |  port | 0x01  | read from port src
IOW     |  rv1  |  r2   | V---- |  port | 0x02  | write to port src
IOC     |  rv1  |  a2   | V--I  |  port | func  | rv1 = port, a2 = atr
  - IOC $CMD, port, atr
  - BSET { adr, size, flg };    - set port I/O buffer
  - BOPEN { iblk, flg };        - open block-device node ['file']
  - BCLOSE desc                 - close block-device node ['file']
  - BREAD { adr, size , ofs };  - read from block-device node ['file']
  - BWRITE { adr, size };       - write to block-device node ['file']
  - BSEEK { desc, orig, ofs };  - seek block-device node ['file'] to position

