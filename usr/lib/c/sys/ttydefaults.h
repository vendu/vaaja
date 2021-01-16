#ifndef __SYS_TTYDEFAULTS_H__
#define __SYS_TTYDEFAULTS_H__

#if (_BSD_SOURCE)

#include <sys/termio.h>

/* default settings on first open */
#define	TTYDEF_IFLAG (BRKINT | ISTRIP | ICRNL | IMAXBEL | IXON | IXANY)
#define TTYDEF_OFLAG (OPOST | ONLCR | OXTABS)
#define TTYDEF_LFLAG (ECHO | ICANON | ISIG | IEXTEN | ECHOE|ECHOKE|ECHOCTL)
#define TTYDEF_CFLAG (CREAD | CS7 | PARENB | HUPCL)
#define TTYDEF_SPEED (B9600)

/* control character defaults */
#define CTRL(x)	     (x&037)
#define	CEOF		 CTRL('d')
#define	CEOL		 ((unsigned)'\377')	/* same as _POSIX_VDISABLE */
#define	CERASE		 0177
#define	CINTR		 CTRL('c')
#define	CSTATUS		 ((unsigned)'\377')	/* same as _POSIX_VDISABLE */
#define	CKILL		 CTRL('u')
#define	CMIN		 1
#define	CQUIT		 034		/* FS, ^\ */
#define	CSUSP		 CTRL('z')
#define	CTIME		 0
#define	CDSUSP		 CTRL('y')
#define	CSTART		 CTRL('q')
#define	CSTOP		 CTRL('s')
#define	CLNEXT		 CTRL('v')
#define	CDISCARD 	 CTRL('o')
#define	CWERASE 	 CTRL('w')
#define	CREPRINT 	 CTRL('r')
#define	CEOT		 CEOF
/* compatibility */
#define	CBRK	 	 CEOL
#define CRPRNT		 CREPRINT
#define	CFLUSH		 CDISCARD

#if defined(TTYDEFCHARS)
cc_t ttydefchars[NCCS] =
{
	CEOF,				// VEOF
	CEOL,				// VEOL
	CEOL,
	CERASE,
	CWERASE,
	CKILL,
	CREPRINT,
	_POSIX_VDISABLE,
	CINTR,
	CQUIT,
	CSUSP,
	CDSUSP,
	CSTART,
	CSTOP,
	CLNEXT,
	CDISCARD,
	CMIN,
	CTIME,
	CSTATUS,
	_POSIX_VDISABLE
};

#endif /* _BSD_SOURCE */

#endif /* __SYS_TTYDEFAULTS_H__ */

