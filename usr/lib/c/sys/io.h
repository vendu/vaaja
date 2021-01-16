#ifndef __SYS_IO_H__
#define __SYS_IO_H__

#include <features.h>
#include <errno.h>
#if (_ZERO_SOURCE)
/* TODO: ioperm() */
#include <sys/zero/syscall.h>
#endif

#define __iodelay()  "outb %%al, $0x80\n"
#define iodelay() __asm__ __volatile__ ("outb %al, $0x80\n")

#if (_ZERO_SOURCE) && !defined(__KERNEL__)
extern ASMLINK long _syscall(long num, long arg1, long arg2, long arg3);
#endif

#if defined(__i386__) || defined(__i486__) || defined(__i586__)         \
    || defined(__i686__) || defined(__x86_64__) || defined(__amd64__)

static __inline__ unsigned char
inb(unsigned short port)
{
    unsigned char ret = 0;

    __asm__ __volatile__ ("inb %1, %b0\n" : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ unsigned char
inb_p(unsigned short port)
{
    unsigned char ret = 0;

    __asm__ __volatile__ ("inb %1, %b0\n"
                          __iodelay()
                          : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ unsigned short
inw(unsigned short port)
{
    unsigned short ret = 0;

    __asm__ __volatile__ ("inw %1, %w0\n" : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ unsigned short
inw_p(unsigned short port)
{
    unsigned short ret = 0;

    __asm__ __volatile__ ("inw %1, %w0\n"
                          __iodelay()
                          : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ unsigned int
inl(unsigned short port)
{
    unsigned int ret = 0;

    __asm__ __volatile__ ("inl %1, %0\n" : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ unsigned int
inl_p(unsigned short port)
{
    unsigned int ret = 0;

    __asm__ __volatile__ ("inl %1, %0\n"
                          __iodelay()
                          : "=a" (ret) : "Nd" (port));

    return ret;

}

static __inline__ void
outb(unsigned char byte, unsigned short port)
{
    __asm__ __volatile__ ("outb %b0, %w1\n" : : "a" (byte), "Nd" (port));

    return;
}

static __inline__ void
outb_p(unsigned char byte, unsigned short port)
{
    __asm__ __volatile__ ("outb %b0, %w1\n"
                          __iodelay()
                          : : "a" (byte), "Nd" (port));

    return;
}

static __inline__ void
outw(unsigned short word, unsigned short port)
{
    __asm__ __volatile__ ("outw %w0, %w1\n" : : "a" (word), "Nd" (port));

    return;
}

static __inline__ void
outw_p(unsigned short word, unsigned short port)
{
    __asm__ __volatile__ ("outw %w0, %w1\n"
                          __iodelay()
                          : : "a" (word), "Nd" (port));

    return;
}

static __inline__ void
outl(unsigned int longword, unsigned short port)
{
    __asm__ __volatile__ ("outl %0, %w1\n" : : "a" (longword), "Nd" (port));

    return;
}

static __inline__ void
outl_p(unsigned int longword, unsigned short port)
{
    __asm__ __volatile__ ("outl %0, %w1\n"
                          __iodelay()
                          : : "a" (longword), "Nd" (port));

    return;
}

#endif /* X86 */

#endif /* __SYS_IO_H__ */

