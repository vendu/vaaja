#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vt/term.h>
#include <vt/vt.h>
#include <ui/ui.h>
#include <ui/text.h>

struct vt   termvt C_ALIGNED(MACH_CL_SIZE);

#if (TEST_VT)
void
vtprintinfo(struct vt *vt)
{
    fprintf(stderr, "VT: fd == %d, master: %s, slave: %s\n",
            vt->atr.fd, vt->atr.masterpath, vt->atr.slavepath);
    fprintf(stderr, "VT: %ld rows x %ld columns\n",
            vt->state.nrow, vt->state.ncol);
    fprintf(stderr, "VT: %ld buffer rows\n",
            vt->textbuf.nbufrow);
    fprintf(stderr, "VT: %ld screen rows\n",
            vt->textbuf.nrow);
    fprintf(stderr, "VT: font %s (%ldx%ld)\n",
            vt->font.name, vt->font.boxw, vt->font.boxh);

    return;
}
#endif /* TEST_VT */

int
main(int argc, char *argv[])
{
    struct winsize  ws;
    long            nrow;
    long            ncol;
//    long       nbufrow;

    vtgetopt(&termvt, argc, argv);
    if (!vtinit(&termvt, argc, argv)) {
        fprintf(stderr, "failed to initialise VT\n");

        exit(1);
    }
    ioctl(0, TIOCGWINSZ, &ws);
    fprintf(stderr, "detacted TTY-size of %dx%d characters\n",
            ws.ws_row, ws.ws_col);
#if 0
    nbufrow = termvt.textbuf.nrow;
    if (!nbufrow) {
        nbufnrow = VTDEFBUFNROW;
    }
#endif
    ncol = termvt.state.ncol;
    if (!ncol) {
        ncol = VT_DEF_COLS;
        termvt.state.ncol = ncol;
    }
    nrow = termvt.state.nrow;
    if (!nrow) {
        nrow = VT_DEF_ROWS;
        termvt.state.nrow = nrow;
    }
    if (!uiinittextbuf(&termvt.textbuf, nrow, ncol, 0)) {

        exit(1);
    }
    vtprintinfo(&termvt);
#if 0
    termvt.state.nrow = 24;
    termvt.state.ncol = 80;
    termvt.textbuf.nrow = VTDEFBUFNROW;
    termvt.textbuf.nrow = 24;
#endif
    termvt.state.w = termvt.state.ncol * termvt.font.boxw;
    termvt.state.h = termvt.state.nrow * termvt.font.boxh;
    vtfree(&termvt);

    exit(1);
}

