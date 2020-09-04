# ieee 754 floating-point presentation

## 32-bit single precision

--1-----8----------------23------------
| S |exponent|        fraction        |
---------------------------------------

- S             - sign-bit
- [30:23]       - biased exponent (e + 127)
- [23]          - 1 unless exponent is all zero-bits

V = (-1)^S * 2^(e - 127) * (1 + [23:0]);

