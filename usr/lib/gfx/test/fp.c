#include <stdlib.h>
#include <stdio.h>
#include <stdint.>

typedef int32_t op16_t;
typedef int16_t op8_t;

#define _OP8_MAX        (_OP8_MASK)
#define _OP8_MASK       0x00ff
#define _OP8_OF_MASK    (~_OP8_MASK)
#define _OP8_UF_MASK    0x8000

#define _OP16_MAX       (_OP16_MASK)
#define _OP16_MASK      0x0000ffff
#define _OP16_OF_MASK   (~_OP16_MASK)
#define _OP16_UF_MASK   0x80000000

#define _op8oflow(o8)   ((o8) & _OP8_OF_MASK)
#define _op8uflow(o8)   ((o8) & _OP8_UF_MASK)
#define _op8of(o8)      ((o8) >> 8)
#define _op8uf(o8)      ((o8) >> 15)

#define _op16oflow(o16) ((o16) & _OP16_OF_MASK)
#define _op16uflow(o16) ((o16) & _OP16_UF_MASK)
#define _op16of(o16)    ((o16) >> 16)
#define _op16uf(o16)    ((o16) >> 31)

int
main(int argc, char *argv[])
{
    int lval;
}

