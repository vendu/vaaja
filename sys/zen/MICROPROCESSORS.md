# The fastest instructions

o logical instructions
- neg           ¬ dest = ~dest + 1;
- not           - dest = !dest;
- orr           - dest = dest | src;
- xorr          - dest = dest ^ src;
- and           - dest = dest & src;
- nor           - dest = dest ~| arg;
- xnor          - dest = src ^~ arg;
- nand          - dest = src ~& arg;
o bitwise instructions
- tst           - dest = src & (1 << arg);
- set           - dest |= 1 << arg;
- clr           - dest &= ~(1 << arg);
- flp           - dest ^= 1 << arg;
o shift instructions
- shl           - dest = dest << src;
- shr           - dest = dest >> src;
- sar           - dest = dest >>> src;
o arithmetic operations
- inc           - dest++;
- dec           - dest--;
- add, uadd     - dest += src;
- sub, usub     - dest -= src;

# Semi-fast instructions

- mul, umul     - dest = dest * src;
- mhi, umhi     - dest = (dest * src) >> 32;
- irp, uirp     - dest = dest * irp(src) >> x;

# Slow instructions
- div, udiv     - dest = dest / src;
- rem, urem     - dest %= dest / src;

# Miscellaneous instructions