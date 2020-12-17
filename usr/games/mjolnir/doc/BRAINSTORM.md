# Mjolnir Gameplay Brainstorming

- D20-set dice

# Quests
- digital bank robbery; iron bar or lock-pick, divert operation, c4, switch
  room, black box, network sniffing, packet deciphering, packet forging to
  access the account databases and NET-traffice
- y2k38; 32-bit timestamp cannat encode times after 03<.14:07 UTC on Jan 19,
  2038. a malicious hacker gets hired to fix bank data systems, but plants a
  trojan to do a few huge transfars from big company accounts to anonymous
  accounts in Europe, then shuts the system down (hopefully wiping the
  transfer logs), then brings the system back up as if nothing happened
- bank tapes, bank database documentation
- mainframes
- direct access to mainframe let's you clone and steal the tape of the core
  war warriors that have ruled the machine
- beating the warrior running on the mainframe at least 5 out of 9 times shall
  gain you masterhood of the mainframe; you shall also acquire the source code
  of the old master warrior
- if you get physical access to a mainframe, you can inspect the tapes, dump
  their contents on punchcards, or steal the tapes.
- other software
  - RCDB      - redcode/warrior debugger
  - RCED      - redcode editor
  - TED       - editor (ASCII/ISO 8859-1 and hexadecimal)
  - SYS       - mainframe operating system
  - IO        - [file-based] I/O utilities
              - mkfs, list, run, copy, move, wipe, read, write
  - TAP       - network sniffer (port, address, log)
  - NET       - network access interface (login, list, copy, wipe)
  - DUMP      - copy program to punchcard
  - ROT13     - 'decipher' file; redcode and certain other files are ciphered
                - the default encryption is ROT13; this is done for fun and to
                  allow relatively easy modifications to the game with some
                  background work
  - HASH      - hash and unhash functions
                - tmhash32(), tmunhash32()

                - better privacy than simple ROT13
  - KRAK      - different malware tools
                - root, single, log(on|off), unlog, crack
  - MEM       - memory logger and editor
                - save(adr, fd, ofs, nrec), load(adr, fd, ofs, nrec)
                - poke(adr, val), peek(adr)
  - CODEBRK   - mastermind game
  - BTLSHIP   - battleship game
                - 10x10 coordinate grid; topright (5, 5); topleft (-5, 5);
                                         downleft (-5, -5); downright (5, -5)
                - ships
                  - 1 4-block aircraft carrier
                  - 2 3-block cruise ships
                  - 3 2-block destroyers
                  - 4 1-block submarines
    - Y38K      - Trojan for bank systems
- terminals
  - equipped with punchcard readers to play core war and run other software
    on in-dungeon mainframes

# Character Skills

# ICE
- ANTI-ICE              - defense against ice           - white, orange
- SOFT-ICE              - non-lethal ice-attacks        - blue, yellow
- HARD-ICE              - lethal ice-attacks            - green, black

# Miscellaneous Skills
- LUCK                  - LUCK factor for things you do
- TIREDNESS             - how tired [and worse at things] you are
- AWARENESS             - opposite of TIREDNESS
- DISTRACTION           - [mental] DISTRACTION factor
- EMPATHY
- HUMANITY              - HUMANITY ZERO is CYBERPSYCHOSIS; player turns into NPC

# Physical Skills
- LOCKS: cur/max        - lock-picking skills; ability to break into buildings
- REFLEX: cur/max       - REFLEX; reaction speed to attacks
- XPERIENCE: cur/max    - XPERIENCE with different things
- HITPOINTS: cur/max    - HITPOINTS (current/max)
- LEVEL                 - character SKILL/XPERIENCE levels
- SILENCE               - helps to gain surprise points in attacks
- IMPLANT               - cybernetic implants for cyborgs, even people

# Budo Skills
- KARATE                - white, orange, blue, yellow, green, brown, black belts
- AIKIDO                - white, yellow, orange, blue, brown, black,
                        - 2rd dan (black, thin gold stripe),
                        - 3rd dan (black, red stripe)
- JUDO                  - white/8, yellow/7, orange/6, green/4, brown/3-1 grade
                          black/1st-5th degree, red&white/6-8, red/9-10

# Mental Skills
- SOCIAL                - SOCIAL engineering
- KARMA                 - combination of good MORALS, ETCHICS, and FAIRNESS

# Deck Setup
- EMP                   - power of electro-magnetic pulses sent to damage decks
- NPROC                 - number of simultaneous processors/processes
- NTH                   - number of per-processor threads
- CLOCK                 - clock frequency
- RAM                   - core memory size
- CACHE                 - cache memory size
- SRAM                  - static memory
- NVRAM                 - static, survives over reboots
- UL                    - upload speed
- DL                    - download speed
- MBPS                  - megabytes per second of storage/retrieval
- CRYPT                 - cryptographical strenght
- NOISE                 - noise to other decks
- JAM                   - jam capability (slow other decks down)
- BOOST                 - boost capability (speed your deck up)
- ISOLATION             - how hard your deck communiciations are to eavesdrop
- FIREWALL              - firewall defense strength

