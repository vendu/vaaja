# Voima Assembler

## Syntax

The Voima assembler shall follow at least mostly AT&T-syntax. This is due to my
feeling it's more universal than many if not most platform-dependent syntaxes,
plus it just makes more sense to me.

.space                  - declare initialised or uninitialised space
label:                  - declare branch target or function
.align <nb>             - align to boundary of nb bytes
.extern <name> <...>    - link with external symbols visible
.global <name> <...>    - declare symbols global

### label               - label-address
### $imm                - 32-bit signed immediate operand follows LDR-op
### #imm                - 32-bit unsigned immediate operand follows LDR-op

_start:
        not     %r0, %r1        ;; r1 = ~0;
        neg     %r1             ;; r1 = -r1;
        or      %r1, %r2        ;; r2 |= r1;
        xor     %r2, %r1        ;; r1 ^= r2;
        ldr     %r1, %r3        ;; r3 = r1;
        ldi     $4, %r4         ;; r4 = 4; // signed operand
        ldi     #5, %r5         ;; r5 = 5  // unsigned operand
        and     %r0, %r1        ;; r1 = 0;

