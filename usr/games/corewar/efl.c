#include <Ecore.h>
#include <Ecore_Evas.h>

static struct zeusefl zeusefl;

void
zeustogglesel(struct zeusefl *efl, XEvent *event)
{
    int  x = event->xbutton.x;
    int  y = event->xbutton.y;
    long pc;

    x /= 5;
    y /= 5;
    pc = y * (efl->w / 5) + x;
    if (!zeussel.bmap) {
        zeussel.bmap = calloc(CWNCORE >> 3, sizeof(uint8_t));
    }
    if (!zeussel.bmap) {
        fprintf(stderr, "memory allocation failure\n");
    } else if (bitset(zeussel.bmap, pc)) {
        clrbit(zeussel.bmap, pc);
    } else {
        setbit(zeussel.bmap, pc);
    }
    zeussel.last = pc;
    zeusdrawsimop(efl, pc);

    return;
}

void
zeusaddsel(struct zeusefl *efl, XEvent *event)
{
    int  x = event->xbutton.x;
    int  y = event->xbutton.y;
    int  lim;
    long pc;

    x /= 5;
    y /= 5;
    pc = y * (efl->w / 5) + x;
    if (!zeussel.bmap) {
        zeussel.bmap = calloc(CWNCORE >> 3, sizeof(uint8_t));
    }
    if (!zeussel.bmap) {
        fprintf(stderr, "memory allocation failure\n");
    } else if (zeussel.last >= 0) {
        if (pc < zeussel.last) {
            lim = zeussel.last;
        } else {
            lim = pc;
            pc = zeussel.last;
        }
        while (pc <= lim) {
            setbit(zeussel.bmap, pc);
            zeusdrawsimop(efl, pc);
            pc++;
        }
        zeussel.last = pc;
    }

    return;
}

void
zeusrun(struct zeusefl *efl, XEvent *event)
{
    long pid = cwmars.curpid;

    cwmars.running = 1;
    while ((cwmars.running) && (cwmars.nturn[pid])) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        cwmars.curpid = pid;
        cwmars.nturn[pid]--;
    }
    if (!cwmars.nturn[pid]) {
        fprintf(stderr, "TIE\n");
        sleep(5);

        exit(0);
    }

    return;
}

void
zeusstop(struct zeusefl *efl, XEvent *event)
{
    cwmars.running = 0;
}

void
zeusstep(struct zeusefl *efl, XEvent *event)
{
    long pid = cwmars.curpid;

    cwmars.running = 0;
    if (cwmars.nturn[pid]--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        cwmars.curpid = pid;
    } else {
        fprintf(stderr, "TIE\n");
        sleep(5);
    }

    return;
}

void
zeusfence(struct zeusefl *efl, XEvent *event)
{
    ;
}

void
zeusclear(struct zeusefl *efl, XEvent *event)
{
    if (zeussel.bmap) {
        memset(zeussel.bmap, 0, CWNCORE >> 3);
    }
    zeussel.last = -1;
    zeusdrawsim(efl);
}

void
zeusexit(struct zeusefl *efl, XEvent *event)
{
    exit(0);
}

int
zeusfindbutton(Window win)
{
    int    id;

    for (id = 0 ; id < ZEUSNBUTTON ; id++) {
        if (zeuseflbuttons.wins[id] == win) {

            break;
        }
    }

    return id;
}

char *
zeusbuttonstring(Window win, int *lenret)
{
    char  *str = NULL;
    int    id = zeusfindbutton(win);

    if (id < ZEUSNBUTTON) {
        str = zeuseflbuttons.strs[id];
        if (str) {
            *lenret = zeuseflbuttons.strlens[id];
        }
    }

    return str;
}

void
zeusinitwinefl(struct zeusefl *efl)
{
    Ecore_Evas *core;
    Evas       *evas;

    if (!ecore_evas_init()) {
        fprintf(stderr, "failed to initialise EFL\n");

        exit(1);
    }
    core = ecore_evas_new(NULL, 0, ZEUSEFLWIDTH, ZEUSEFLHEIGHT, NULL);
    if (!core) {
        fprintf(stderr, "failed to create Evas\n");

        exit(1);
    }
    efl->core = core;
    ecore_evas_title_set(core, "Zeus");
    ecore_evas_show(core);
    efl->evas = ecore_evas_get(core);
}

void
zeusdrawsimop(struct zeusefl *efl, long pc)
{
    struct cwinstr *op;
    int             row = pc / ZEUSSIMNCOL;
    int             col = pc - row * ZEUSSIMNCOL;
    int             x = col * 5;
    int             y = row * 5;

    op = &cwmars.optab[pc];
    if ((zeussel.bmap) && bitset(zeussel.bmap, pc)) {
        XFillRectangle(efl->disp, efl->pixbuf,
                       efl->selgc,
                       x, y,
                       4, 4);
    } else if (op->op == CWOPDAT) {
        if (!*((uint64_t *)op)) {
            XFillRectangle(efl->disp, efl->pixbuf,
                           efl->datgc,
                           x, y,
                           4, 4);
        } else if (op->pid) {
            XFillRectangle(efl->disp, efl->pixbuf,
                           efl->prog2datgc,
                           x, y,
                           4, 4);
        } else {
            XFillRectangle(efl->disp, efl->pixbuf,
                           efl->prog1datgc,
                           x, y,
                           4, 4);
        }
    } else if (op->pid) {
        XFillRectangle(efl->disp, efl->pixbuf,
                       efl->prog2gc,
                       x, y,
                       4, 4);
        
    } else {
        XFillRectangle(efl->disp, efl->pixbuf,
                       efl->prog1gc,
                       x, y,
                       4, 4);
    }
    XCopyArea(efl->disp, efl->pixbuf, efl->simwin,
              efl->bggc,
              x, y,
              4, 4,
              x, y);

    return;
}

void
zeusdrawsim(struct zeusefl *efl)
{
    long pc;

    for (pc = 0 ; pc < CWNCORE ; pc++) {
        zeusdrawsimop(efl, pc);
    }
    XCopyArea(efl->disp, efl->pixbuf, efl->simwin,
              efl->bggc,
              0, 0,
              efl->simw, efl->simh,
              0, 0);

    return;
}

