#ifndef DECK_VM_H
#define DECK_VM_H

/* deck virtual machine */

#define DECK_VM_MEM_DEF_SIZE    (8 * 1024 * 1024)   // 8 megabytes

#define DECK_VM_RESET           0x00
#define DECK_VM_RUN             0x01
#define DECK_VM_STOP            0x02
#define DECK_VM_HALT            0x03

struct deckvm {
    int8_t                     *mem;        // virtual machine memory
    uint32_t                    memsize;    // size of memory in bytes
    uint8_t                     state;      // RESET, RUN, STOP, HALT
};

#endif /* DECK_VM_H */

