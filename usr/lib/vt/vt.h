#ifndef __VT_VT_H__
#define __VT_VT_H__

#include <vt/conf.h>
#include <vt/color.h>
#include <stdint.h>
#include <mach/param.h>
#include <mach/types.h>
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <vt/term.h>
#include <ui/ui.h>
#include <ui/text.h>

#define VT_BUF_SIZE             MACH_PAGE_SIZE

#define VT_DEF_MODE             0x00000000U
#define VT_DEF_FG_COLOR         0xffffffffU
#define VT_DEF_BG_COLOR         0xff000000U
#define VT_DEF_TEXT_ATR         0x00000000U

/* Unicode plans not to add characters beyond 0x10ffff */
#define VT_UCS_MASK             0x001fffffU
#define VT_ATR_MASK             0xffe00000U

/* character attribute bits */
#define VT_BRIGHT               0x80000000
#define VT_DIM                  0x40000000
#define VT_UNDERSCORE           0x20000000
#define VT_BLINK                0x10000000
#define VT_REVERSE              0x08000000
#define VT_HIDDEN               0x04000000
#define VT_ANTIALIAS            0x02000000 // antialiased text (interpolation)
#define VT_DRAW_BG              0x01000000 // draw text background
#define VT_DRAW_FG              0x00800000 // draw text foreground
#define VT_XTERM_256            0x00400000 // 256-color xterm mode
#define VT_FG_MASK              0x000000ff // standard or 256-color xterm palette entry
#define VT_BG_MASK              0x0000ff00 // standard or 256-color xterm palette entry
#define VT_FG_256_BIT           0x00000100 // foreground is xterm color
#define VT_BG_256_BIT           0x00020000 // foreground is xterm color

#define VT_XTERM_256_BIT        0x100 // xterm 256-color palette entry

/* Unicode character plus attributes such as underline */
//typedef int32_t vtchar_t;
/* character and its 32-bit ARGB-color */
//typedef int64_t vttext_t;

#define ZEN_RING_ITEM           int32_t
#define ZEN_RING_INVAL          0x00000000
#include <zen/ring.h>

#define VT_ESC                  '\033'
/* sequence postfixes after ESC */
#define VT_CSI                  '['
#define VT_FONT_G0              '('
#define VT_FONT_G1              ')'
#define VT_HASH                 '#'
/* sequences prefixed with ESC */
#define VT_CURSOR_UP            'A'
#define VT_CURSOR_DOWN          'B'
#define VT_CURSOR_RIGHT         'C'
#define VT_CURSOR_LEFT          'D'
#define VT_SPEC_CHARSET         'F'
#define VT_ASCII_CHARSET        'G'
#define VT_CURSOR_HOME          'H'
#define VT_REV_LINEFEED         'I'
#define VT_CLR_TO_SCR_END       'J'
#define VT_CLR_TO_LINE_END      'K'
#define VT_CURSOR_ADR           'Y'

struct vtatr {
    int         fd;             // master PTY file descriptor
    char       *masterpath;     // master teletype device
    char       *slavepath;      // slave teletype device
    char       *termvar;
    char       *ifsvar;
};

struct vtstate {
    long        col;            // current column
    long        row;            // current column
    long        nrow;           // # of screen rows
    long        ncol;           // # of screen columns
    long        w;              // width in characters
    long        h;              // height in characters
    uint64_t    mode;           // private mode etc. mask
    uint64_t    flags;          // modifier and button mask
    int32_t     fgcolor;        // foreground text color
    int32_t     bgcolor;        // background text color
    uint32_t    textatr;        // current text attributes
    uint32_t   *tabmap;         // TAB-position bitmap
    long        scrolltop;      // scrolling region top row
    long        scrollbottom;   // scrolling region bottom row
};

struct vtdevbuf {
    struct ringbuf  in;         // input ring-buffer
    struct ringbuf  out;        // output ring-buffer
};

struct vtiobuf {
    long            ninp;       // # of items in input buffer
    void           *inptr;      // current input pointer
    void           *inbuf;      // input buffer
    long            nout;       // # of items in output buffer
    void           *outptr;     // current output pointer
    void           *outbuf;     // output buffer
};

struct vtcolormap {
    void           *deftab;     // default colors
    void           *xtermtab;   // xterm colors
};

struct vtsavecurs {
    long            row;
    long            col;
};

typedef ssize_t __termiofunc(int fd, void *buf, size_t nbyte);
typedef int     __termioop(int fd, long cmd, void *buf);
/* values for the flg member */
#define VT_BUF_DIRTY        (1 << 0)    // terminal buffer not synchronized
#define VT_BUF_READ         (1 << 1)    // terminal has pending read operations
#define VT_BUF_WRITE        (1 << 2)    // terminal has pending write operations
struct vtbuf {
    volatile long       lk;         // mutex
    long                flg;        // VT_BUF_DIRTY, VT_BUF_READ, VT_BUF_WRITE
    struct termiobuf   *rdqueue;    // read queue buffer structure
    struct termiobuf   *wrqueue;    // write queue buffer structure
    __termiofunc       *chop;       // getch() (read()) or putch() (write())
    fd_set             *wrfdset;    // write fd_set for select()
    fd_set             *rdfdset;    // read fd_set for select()
    fd_set             *errfdset;   // error fd_set for select()
    struct timeval      timeout;    // possible timeout for select()
};

struct vtscreen {
    struct vt          *vttab;      // basic terminal attributes (VT_SCREENS)
    int                *fdtab;      // TERM_SCREEN_FDS file descriptors
    struct vtbuf        buf;        // virtual terminal buffer structure
    size_t              nrow;       // # of rows in rowtab
    size_t              nscrrow;    // # of rows in scrtab
    struct vtrow      **rowtab;     // current I/O buffer
    struct vtrow      **scrtab;     // current screen rendition
};

struct vt {
    struct vtbuf       *buf;
    char               *path;
#if (TERM_XORG)
    int                 connfd;     // X connection file descriptor
#endif
    struct vtatr        atr;        // terminal attributes
    struct vtstate      state;      // terminal status
#if defined(__zen__)
    struct vtdevbuf     devbuf;     // input and output buffers
#else
    struct vtiobuf      iobuf;
#endif
    struct uitextbuf    textbuf;    // text and rendition buffers
    struct ui           ui;         // user interface
    struct uifont       font;       // default font
    struct vtcolormap   colormap;   // terminal colormaps
    struct vtesctabs   *esctabs;    // escape sequence interface
    struct vtsavecurs   savecurs;   // saved cursor and attributes
};

typedef void vtescfunc_t(struct vt *, m_word_t, m_word_t *);
struct vtesctabs {
    uint8_t             escmap[32];
    uint8_t             csimap[32];
    uint8_t             hashmap[32];
    vtescfunc_t        *escfunctab[256];
    vtescfunc_t        *csifunctab[256];
    vtescfunc_t        *hashfunctab[256];
};

#define vtisesccmd(vt, c)           bitset((vt)->esctabs->escmap, c)
#define vtiscsicmd(vt, c)       bitset((vt)->esctabs->csimap, c)
#define vtishashcmd(vt, c)      bitset((vt)->esctabs->hashmap, c)
#define vtsetesccmd(vt, c)      setbit((vt)->esctabs->escmap, c)
#define vtsetcsicmd(vt, c)      setbit((vt)->esctabs->csimap, c)
#define vtsethashcmd(vt, c)     setbit((vt)->esctabs->hashmap, c)
#define vtsetescfunc(vt, c, f)                                          \
    (vtsetesccmd(vt, c), (vt)->esctabs->escfunctab[(c)] = (f))
#define vtsetcsifunc(vt, c, f)                                          \
    (vtsetcsicmd(vt, c), (vt)->esctabs->csifunctab[(c)] = (f))
#define vtsethashfunc(vt, c, f)                                         \
    (vtsethashcmd(vt, c), (vt)->esctabs->hashfunctab[(c)] = (f))

#define vtdefcolor(i)     (((int32_t *)((vt)->colormap.deftab))[(i)])
#define vtxtermcolor(i)   (((int32_t *)((vt)->colormap.xtermtab))[(i)])
#define vtfgtodefcolor(i) ((i) - 30)
#define vtbgtodefcolor(i) ((i) - 40)

extern struct vtesctabs  vtesctabs;
extern char             *vtkeystrtab[128];

struct vt * vtrun(struct vt *vt);
void        vtgetopt(struct vt *vt, int argc, char *argv[]);
struct vt * vtinit(struct vt *vt, int argc, char *argv[]);
void        vtinitesc(struct vt *vt);
void        vtfree(struct vt *vt);
long        vtinitbuf(struct vt *vt);
void        vtfreebuf(struct vt *vt);

#endif /* __VT_VT_H__ */

