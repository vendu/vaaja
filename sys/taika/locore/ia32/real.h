#ifndef __UNIT_IA32_REAL_H__
#define __UNIT_IA32_REAL_H__

void   realstart(void);
void * realend;

struct realregs {
    int16_t di;
    int16_t si;
    int16_t bp;
    int16_t sp;
    int16_t bx;
    int16_t dx;
    int16_t cx;
    int16_t ax;
};

#endif /* __UNIT_IA32_REAL_H__ */

