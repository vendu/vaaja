#ifndef __VT_TERM_H__
#define __VT_TERM_H__

#include <vt/conf.h>
#include <stdint.h>
#include <unistd.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <sys/select.h>

#define TERM_INPUT_FD       0
#define TERM_OUTPUT_FD      1
#define TERM_ERROR_FD       2
#define TERM_TTY_FD         3
#define TERM_FDS            4

#define TERM_INBUF_SIZE     (8 * MACH_PAGE_SIZE)
#define TERM_OUTBUF_SIZE    (8 * MACH_PAGE_SIZE)

//#include <vt/vt.h>
//#include <vt/io.h>

#endif /* __VT_TERM_H__ */

