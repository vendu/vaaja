#ifndef __TERMIO_H__
#define __TERMIO_H__

typedef unsigned char  cc_t;
typedef unsigned short tcflag_t;

#define NCCS 32

struct termio {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t     c_cc[NCCS];
};

/* c_cc subscript names */
#define VINTR   1
#define VQUIT   2
#define VERASE  3
#define VTIME   4
#define VMIN    5

/* c_iflag */
#define IGNBRK  0x01
#define ICRNL   0x02
#define IXON    0x04

/* c_oflag */
#define ONLCR   0x01
#define TABDLY  0x02
#define TAB3    0x04

/* c_cflag */
#define CBAUD   0x01
#define B9600   0x02

#define ISIG    1
#define ICANON  2
#define ECHO    3
#define ICSETAW 4
#define ICSETAF 5

#endif /* __TERMIO_H__ */

