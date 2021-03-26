#ifndef __UNIT_ARM_TRAP_H__
#define __UNIT_ARM_TRAP_H__

/* processor modes */
#define TRAPUSR32 0x10   // user mode
#define TRAPFIQ32 0x11   // FIQ mode
#define TRAPIRQ32 0x12   // IRQ mode
#define TRAPSVC32 0x13   // supervisor mode
#define TRAPABT32 0x17   // abort mode
#define TRAPUND32 0x1b   // undefined instruction mode
#define TRAPSYS32 0x1f   // system mode
#define TRAPDISAB 0xc0   // interrupts disabled

#endif /* __UNIT_ARM_TRAP_H__ */

