#ifndef __VT_CONF_H__
#define __VT_CONF_H__

#define TERM_XORG       1
#define TERM_SCREENS    2
#define TERM_SCREEN_VTS 4

#define VT_DEBUG_ESC    1

#define VT_DEF_FONT     "-misc-proggycleanttsz-medium-r-normal--0-0-0-0-c-0-iso8859-1"
#define VT_2ND_FONT     "fixed"
#define VT_DEF_ROWS     24
#define VT_DEF_COLS     80

#define VT_XORG         1
#define VT_POSIX_OPENPT 1
#define VT_GETPT        0
#define VT_DEV_PTMX     0
#define VT_PTMX_DEVICE  "/dev/ptmx"
#if (VT_POSIX_OPENPT)
#define _XOPEN_SOURCE   600
#endif
#if (VT_GETPT)
#define _GNU_SOURCE     1
#endif

#endif /* __VT_CONF_H__ */

