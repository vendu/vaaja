# Mjolnir Decks; virtual portable computer with a terminal-like text interface

## Hardware Configurations
- up to 32 cores, each running at about 64 MIPS
- up to 32x4MB RAM-chips

## PROG-folder; Software
- ENC32             - encode using a 32-bit hash algorithm
- REV32             - decode data encoded with ENC32
- TAP               - sniff, analyze, forge, and record network data packets
- DEB               - DPL (Deck Programming Language) debugger
- DPL               - DPL compiler
  - PKT             - packet analysis and network development tools
    - crccalc()     - calculate package CRC-checksum (from IP v4)
    - getsrcadr()   - read current packet's source address
    - getdestadr()  - read current packet's destination address
    - getsrcport()  - read current packet's source port
    - getdestport() - read current packet's destination port
    - pushpf()      - push packet filter to chain; handle in LIFO-order
    - logpkt()      - log packet info in text
    - savepkt()     - record packet binary data
    - recvpkt()     - receive packet
    - sendpkt()     - transmit packet
    - copypkt()     - copy packet binary data (to another chain) verbatim
- NOTES             - automatically generated and hand-written notes on the
                      levels; maps included
- MAGIC             - your spell and magic book
- KRAK              - a Mjolnir subgame where you try to penetrate into a
                      foreign system. Your success depends on you skills as well
                      as the KRAK-modules you have in your hold - I might try
                      to make it relatively easy to program these Deck
                      Programming Language (DPL) programs with the aid of the
                      TAP and possibly other interfaces.

