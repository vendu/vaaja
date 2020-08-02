#include <stdint.h>
#include <mjolnir/mjol.h>

static uint8_t  mjolchrcnttab[MJOLNIR_DEF_NLVL][128];
static uint8_t  mjolobjcnttab[MJOLNIR_DEF_NLVL][128];

void
mjoladdchr(int lvl, int ch)
{
    uint8_t u8 = ch & 0x7f;
    
    mjolchrcnttab[lvl][u8]++;

    return;
}

void
mjoladdobj(int lvl, int ch)
{
    uint8_t u8 = ch & 0x7f;
    
    mjolobjcnttab[lvl][u8]++;

    return;
}

