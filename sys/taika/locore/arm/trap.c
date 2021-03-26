#include <stdint.h>

#if 0

#define TRAPRESET 0x00 /* reset */
#define TRAPUNDEF 0x01 /* undefined instruction */
#define TRAPSWI   0x02 /* software interrupt */
#define TRAPPREF  0x03 /* prefetch abort */
#define TRAPDATA  0x04 /* data abort */
#define TRAPRES   0x05 /* not assigned */
#define TRAPIRQ   0x06 /* interrupt request */
#define TRAPFIQ   0x07 /* fast interrupt request */
#define NTRAP     8    /* # of entries in vector table */

/* use handlers a'la LDR pc [pc, #offset] */
extern uint32_t trapvec[];

void
intinit(void)
{
    /* initialize stack pointers */
    /* enable vector table at 0x00000000 */
//    trapinit(); // assembly routine
}

#endif

