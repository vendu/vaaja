/* zero c library terminal I/O interface */

#if !defined(__KERNEL__)

#ifndef __TERMIOS_H__
#define __TERMIOS_H__

#include <features.h>
#if (USEUNIX98) || (USEXOPEN2K8)
#include <sys/types.h>
#endif
#include <bits/termios.h>

extern speed_t cfgetispeed(const struct termios *term);
extern speed_t cfgetospeed(const struct termios *term);
extern int     cfsetispeed(struct termios *term, speed_t);
extern int     cfsetospeed(struct termios *term, speed_t);
extern int     tcdrain(int);
extern int     tcflow(int, int);
extern int     tcflush(int, int);
extern int     tcgetattr(int, struct termios *term);
extern int     tcsendbreak(int, int);
extern int     tcsetattr(int, int, const struct termios *term);
#if (_BSD_SOURCE)
void           cfmakeraw(struct termios *term);
void           cfsetspeed(struct termios *term, speed_t speed); 
#endif

typedef unsigned char cc_t;
typedef unsigned long speed_t;
typedef unsigned long tcflag_t;

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t     c_cc[NCCS];
};

/* c_cc field */
#define VMIN     VEOF
#define VEOF     0U
#define VEOL     1U
#define VERASE   2U
#define VINTR    3U
#define VKILL    4U
#define VQUIT    5U
#define VSTART   6U
#define VSTOP    7U
#define VSUSP    8U
#define VTIME    VEOL
#define VSWTC    9U
#define VREPRINT 10U
#define VDISCARD 11U
#define VWERASE  12U
#define VLNEXT   13U
#define VEOL2    14U
#define NCCS     16U

/* c_iflag field */
#define BRKINT   0x00000001U
#define ICRNL    0x00000002U
#define IGNBRK   0x00000004U
#define IGNCR    0x00000008U
#define IGNPAR   0x00000010U
#define INLCR    0x00000020U
#define INPCK    0x00000040U
#define ISTRIP   0x00000080U
#define IXANY    0x00000100U
#define IXOFF    0x00000200U
#define PARMRK   0x00000400U
#define IUCLC    0x00000800U
#define IMAXDEL  0x00001000U
#define IUTF8    0x00002000U

/* c_oflag field */
#define OPOST    0x00000001U
#define ONLCR    0x00000002U
#define OCRNL    0x00000004U
#define ONOCR    0x00000008U
#define ONLRET   0x00000010U
#define OFDEL    0x00000020U
#define OFILL    0x00000040U
#if (_XOPEN_SOURCE)
#define NLDLY    0x00000080U
#define NL0      0x00000000U
#define NL1      0x00000080U
#define CRDLY    0x00000300U
#define CR0      0x00000000U
#define CR1      0x00000100U
#define CR2      0x00000200U
#define TABDLY   0x00000c00U
#define TAB0     0x00000000U
#define TAB1     0x00000400U
#define TAB2     0x00000800U
#define TAB3     0x00000c00U
#define BSDLY    0x00001000U
#define BS0      0x00000000U
#define BS1      0x00001000U
#define FFDLY    0x00002000U
#define FF0      0x00000000U
#define FF1      0x00002000U
#endif
#define VTDLY    0x00004000U
#define VT0      0x00000000U
#define VT1      0x00004000U

#define XTABS    TAB3

#if (_BSD_SOURCE)
#define CCEQ(val, c) ((c) == val && (val) != _POSIX_VDISABLE)
#endif

#endif /* !__KERNEL__ */

#endif /* __TERMIOS_H__ */

