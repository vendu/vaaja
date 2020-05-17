/* valhalla virtual machine compile-time configuration */

#ifndef __VAS_CONF_H__
#define __VAS_CONF_H__

#define VASDEBUG         1
#define VASBUF           0
#define VASMMAP          1
#define V0               1
#define VAS_LINE_BUFSIZE 65536
#define VASPREPROC       1
#define VASDB            0
#define VASALIGN         0
#define PAGESIZE         4096
#define VAS_READ_BUFS    32

#define vasadrtoptr(adr) (&v0vm->mem[adr])

/*
 * choose input file buffering scheme
 * - default is to use <stdio.h> functionality
 */
#ifndef VASMMAP
/* use mmap()'d regions for input file buffers */
#define VASMMAP  1
#endif
#ifndef VASBUF
/* use explicit I/O buffering of input files */
#define VASBUF   0
#endif

#endif /* __VAS_CONF_H__ */

