#ifndef __KERN_UNIT_X86_PIT_H__
#define __KERN_UNIT_X86_PIT_H__

void pitinit(void);
void pitsleep(unsigned long msec);

/* support package for the 8253 timer chip */

#define PITHZ       (1193182L / HZ)
#define PITCMD      (PITDUALBYTE | PITWAVEGEN)

/* only use channel 0 */
#define PITCHAN0    0x40 /* data port */
#define PITCHAN1    0x41
#define PITCHAN2    0x42
#define PITCTRL     0x43 /* mode/command register */
#define PITDELAY    0x60
#define PITCTRL2    0x61 /* PC speaker port / keyboard controller */
/* modes/commands */
#define PITTERMCNT  0x00 /* interrupt on terminal count */
#define PITONESHOT  0x01 /* hardware re-triggerable one shot */
#define PITRATEGEN  0x02 /* rate generator */
#define PITWAVEGEN  0x03 /* square wave generator */
#define PITSOFTSTB  0x04 /* software triggered strobe */
#define PITHARDSTB  0x05 /* hardware triggered strobe */
#define PITLOBYTE   0x10
#define PITHIBYTE   0x20
#define PITDUALBYTE 0x30
#define PITREADBACK 0xc0

static __inline__ unsigned long
divu1193182(unsigned long uval)
{
        unsigned long long mul = UINT64_C(0xe1);
        unsigned long long res = uval;
        unsigned long      cnt = 20;

        res *= mul;
        res >>= cnt;
        uval = (unsigned long)res;

        return uval;
}

#define pitsethz(hz, chan)                                              \
    do {                                                                \
        long _hz = divu1193182(hz);                                     \
                                                                        \
        outb(_hz & 0xff, (chan));                                       \
        inb(PITDELAY);                                                  \
        outb(_hz >> 8, (chan));                                         \
    } while (0)

#endif /* __KERN_UNIT_X86_PIT_H__ */

