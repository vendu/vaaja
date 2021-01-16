uint8_t                         __bitswaptable[256];

static uint8_t
_bitswap(uint8_t u8)
{
    uint8_t                     val = 0;

    if (u8 & 0x80) {
        val |= 0x01;
    }
    if (u8 & 0x40) {
        val |= 0x02;
    }
    if (u8 & 0x20) {
        val |= 0x04;
    }
    if (u8 & 0x10) {
        val |= 0x08;
    }
    if (u8 & 0x08) {
        val |= 0x10;
    }
    if (u8 & 0x04) {
        val |= 0x20;
    }
    if (u8 & 0x02) {
        val |= 0x40;
    }
    if (u8 & 0x01) {
        val |= 0x80;
    }

    return val;
}

void
genbitswaptable(void)
{
    long                        val;

    for (val = 0 ; val < 256 ; val++) {
        __bitswaptable[val] = _bitswap(val);
    }

    return;
}

