#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>

char       _inaddrstr[16] ALIGNED(CLSIZE);
const char _asctodectab[10]
= {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9'
};

char *
inet_ntoa(struct in_addr *adr)
{
    uint8_t *u8ptr = (unsigned char *)_inaddrstr;
    uint8_t *ptr = (uint8_t *)adr;
    int      len = 0;
    long     val;
    long     tmp1;
    long     tmp2;
    long     tmp3;
    long     tmp4;
    long     tmp5;
    int      ndx;

    for (ndx = 0 ; ndx < 4 ; ndx++) {
        val = ptr[ndx];
//        size = 1;
        tmp1 = modu10(val);         // ptr[0] % 10
        tmp2 = divu10(val);         // ptr[0] / 10
        tmp3 = divu100(val);        // ptr[0] / 100
        tmp4 = modu10(tmp2);        // (ptr[0] / 10) % 10
        tmp5 = modu10(tmp3);        // (ptr[0] / 100) % 10
        if (val >= 100) {
            len = 3;
        } else if (val >= 10) {
            len = 2;
        } else {
            val = 1;
        }
        switch (len) {
            case 3:
                u8ptr[2] = _asctodectab[tmp1];
                u8ptr[1] = _asctodectab[tmp4];
                u8ptr[0] = _asctodectab[tmp5];

                break;
            case 2:
                u8ptr[1] = _asctodectab[tmp1];
                u8ptr[0] = _asctodectab[tmp4];

                break;
            case 1:
                u8ptr[0] = _asctodectab[tmp1];

            default:

                break;
        }
        u8ptr += len;
        if (ndx < 3) {
            u8ptr[0] = '.';
        } else {
            u8ptr[0] = '\0';
        }
    }

    return _inaddrstr;
}

int
inet_aton(const char *cptr, struct in_addr *adr)
{
    uint8_t *aptr = (uint8_t *)adr;
    long     n = 0;
    long     val;

    while (1) {
        val = 0;
        if (!isdigit(cptr[0])) {

            return 0;
        }
        while (isdigit(cptr[0])) {
            val *= 10;
            val += cptr[0];
            cptr++;
        }
        n++;
        if (cptr[0] == '.') {
            if (val > 0xff) {

                return 0;
            } else {
                *aptr = (uint8_t)val;
                cptr++;
                aptr++;
            }
        } else if (!cptr[0] && n == 4) {

            return 1;
        } else {

            return 0;
        }
    }

    return 0;
}

