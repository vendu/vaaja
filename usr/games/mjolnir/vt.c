#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <mjolnir/vt.h>

#define _min(a, b)  ((a) < (b) ? (a) : (b))

struct vtparm mjolvtparm;

int
vtgetsize(int *w, int *h)
{
    struct winsize  ws;

    if (ioctl(0, TIOCGWINSZ, &ws) != -1) {
        *w = ws.ws_col;
        *h = ws.ws_row;
        
        return 1;
    }
    
    return 0;
}

void
vtreset(void)
{
    int     i;
    int     j;
    int     k;

    k = 0;
    for (i = 0 ; i < 11; i++) {
        for (j = 0 ; j < 10 ; j++) {
            k += j;
            if (k <= 108) {
                printf(VT_RESET_STRING, k, k);
            } else {
                
                break;
            }
        }
        printf("\n");
    }
    
    return;
}

void
vtsetparm(int num)
{
    printf(VT_SET_PARM_STRING, num);

    return;
}

void
vtsetg0charset(int map)
{
    printf(VT_SET_G0_CHARSET_STRING, map);

    return;
}

int
vtinit(struct vtparm *parm)
{
    vtreset();
    if (!vtgetsize(&parm->w, &parm->h)) {

        return 0;
    }
    vtsetparm(VT_RESET_DEFAULTS);

    return 1;
}

void
vt0setdeffg(void)
{
    vtsetparm(VT_SET_DEFAULT_FG_COLOR);

    return;
}
    
void
vt0setdefbg(void)
{
    vtsetparm(VT_SET_DEFAULT_BG_COLOR);

    return;
}

void
vtsetcolor(const char *str)
{
    printf(VT_SET_COLOR_STRING, str);

    return;
}

void
vgaset256color(int cmd, int num)
{
    printf(VT_SET_256_COLOR_STRING, cmd, num);

    return;
}

void
vgaset24bitcolor(int cmd, int r, int g, int b)
{
    printf(VT_SET_24BIT_COLOR_STRING, cmd, r, g, b);

    return;
}

void
vtsetrgbcolor(int num, int r, int g, int b)
{
    printf(VT_SET_RGB_COLOR_STRING, num, r, g, b);

    return;
}

void
vtset80col(void)
{
    printf(VT_SET_80_COL_STRING);

    return;
}

void
vtset132col(void)
{
    printf(VT_SET_132_COL_STRING);

    return;
}

void
vtsetwintitle(const char *str)
{
    printf(VT_SET_WIN_TITLE_STRING, str);

    return;
}

void
vtgoto(int x, int y)
{
    mjolvtparm.x = x;
    mjolvtparm.y = y;
    printf("\033[%d;%df", x, y);

    return;
}

void
vtmoveleft(int n)
{
    n = _min(n, mjolvtparm.x);
    printf("\033[%dD", n);
    mjolvtparm.x -= n;

    return;
}

void
vtmoveright(int n)
{
    n = _min(n, mjolvtparm.w - mjolvtparm.x - 1);
    printf("\033[%dC", n);
    mjolvtparm.x += n;

    return;
}

void
vtmoveup(int n)
{
    n = _min(n, mjolvtparm.y);
    printf("\033[%dA", n);
    mjolvtparm.y -= n;

    return;
}

void
vtmovedown(int n)
{
    n = _min(n, mjolvtparm.w - mjolvtparm.y - 1);
    printf("\033[%dB", n);
    mjolvtparm.y += n;

    return;
}

void
vtclrmovetopleft(void)
{
    printf("\033[2]");

    return;
}

void
vtclrtoeol(void)
{
    printf("\033[K");

    return;
}

void
vclrscr(void)
{
    printf("\033[2K");
}
