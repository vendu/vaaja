#include <corewar/conf.h>

#if defined(ZEUS) && defined(ZEUSSDL)

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <zero/trix.h>
#include <corewar/rc.h>
#include <corewar/cw.h>

void    zeusrun(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event);
void    zeusstop(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event);
void    zeusstep(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event);
void    zeusdrawsim(struct zeussdl *sdl);

#define SDL_BACKGROUND_IMAGE    "../../share/pix/fractalchip-small.bmp"

extern struct cwmars            g_cwmars;
extern struct zeussel           g_zeussel;

struct zeussdl                  zeussdl;
struct zeussdlguiwin            zeussdlguiwins[CW_SDL_GUI_BUTTONS]
= {
    { "run", zeusrun },
    { "stop", zeusstop },
    { "step", zeusstep }
};
struct zeussdlbuttonimgs        zeussdlbuttonimgs;
SDL_Rect                        zeussdlrects[CW_SDL_VERT_RECTS * CW_SDL_HORIZ_RECTS];
Uint32                          zeussimwinid;
Uint32                          zeusguiwinid;

void
zeusrun(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event)
{
    long pid = g_cwmars.curpid;

    g_cwmars.running = 1;
    while ((g_cwmars.running) && (g_cwmars.nturn[pid])) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
        g_cwmars.nturn[pid]--;
    }
    if (!g_cwmars.nturn[pid]) {
        fprintf(stderr, "TIE\n");
        sleep(5);

        exit(0);
    }

    return;
}

void
zeusstop(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event)
{
    g_cwmars.running = 0;

    return;
}

void
zeusstep(C_UNUSED struct zeussdl *sdl, C_UNUSED SDL_Event *event)
{
    long pid = g_cwmars.curpid;

    g_cwmars.running = 0;
    if (g_cwmars.nturn[pid]--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    } else {
        fprintf(stderr, "TIE\n");
        sleep(5);
    }

    return;
}

void
zeustogglesel(struct zeussdl *sdl, SDL_Event *event)
{
    int  x = event->button.x;
    int  y = event->button.y;
    long pc;

    x /= 4;
    y /= 4;
    pc = y * (CW_SDL_WIDTH / 4) + x;
    if (bitset(g_zeussel.bmap, pc)) {
        clrbit(g_zeussel.bmap, pc);
    } else {
        setbit(g_zeussel.bmap, pc);
    }
    g_zeussel.last = pc;
    zeusdrawsimop(sdl, pc, 1);

    return;
}

void
zeusaddsel(struct zeussdl *sdl, SDL_Event *event)
{
    int  x = event->button.x;
    int  y = event->button.y;
    long lim;
    long pc;

    x /= 4;
    y /= 4;
    pc = y * (CW_SDL_WIDTH / 4) + x;
    if (g_zeussel.last >= 0) {
        if (pc < g_zeussel.last) {
            lim = g_zeussel.last + 1;
        } else {
            lim = pc;
            pc = g_zeussel.last + 1;
        }
        while (pc < lim) {
            setbit(g_zeussel.bmap, pc);
            zeusdrawsimop(sdl, pc, 1);
            pc++;
        }
        g_zeussel.last = pc;
    }

    return;
}

void
zeusclear(struct zeussdl *sdl, C_UNUSED SDL_Event *event)
{
    g_zeussel.last = -1;
    zeusdrawsim(sdl);
}

void
sdlcleanup(void)
{
    if (zeussdlbuttonimgs.norm) {
        SDL_FreeSurface(zeussdlbuttonimgs.norm);
    }
    if (zeussdlbuttonimgs.hover) {
        SDL_FreeSurface(zeussdlbuttonimgs.hover);
    }
    if (zeussdlbuttonimgs.click) {
        SDL_FreeSurface(zeussdlbuttonimgs.click);
    }
    if (zeussdl.surface) {
        SDL_FreeSurface(zeussdl.surface);
    }
    if (zeussdl.bmp) {
        SDL_FreeSurface(zeussdl.bmp);
    }
    if (zeussdl.texture) {
        SDL_DestroyTexture(zeussdl.texture);
    }
    if (zeussdl.rend) {
        SDL_DestroyRenderer(zeussdl.rend);
    }
    if (zeussdl.gui) {
        SDL_DestroyWindow(zeussdl.gui);
    }
    if (zeussdl.win) {
        SDL_DestroyWindow(zeussdl.win);
    }
    SDL_Quit();

    return;
}

void
sdlinitrects(SDL_Rect *tab)
{
    SDL_Rect   *rect = tab;
    int         gridw = CW_SDL_RECT_SIZE * CW_SDL_HORIZ_RECTS;
    int         gridh = CW_SDL_RECT_SIZE * CW_SDL_VERT_RECTS;
    int         x;
    int         y;

    rect = tab;
    for (y = 0 ; y < gridh ; y += CW_SDL_RECT_SIZE) {
        for (x = 0 ; x < gridw ; x += CW_SDL_RECT_SIZE) {
            rect->x = x;
            rect->y = y;
            rect->w = CW_SDL_RECT_SIZE;
            rect->h = CW_SDL_RECT_SIZE;
            rect++;
        }
    }
    SDL_FillRects(zeussdl.surface,
                  tab,
                  CW_SDL_VERT_RECTS * CW_SDL_HORIZ_RECTS,
                  SDL_MapRGB(zeussdl.surface->format, 255, 255, 255));

    return;
}

int
sdlmksurface(void)
{
    zeussdl.surface = SDL_CreateRGBSurface(0,
                                           CW_SDL_HORIZ_RECTS * CW_SDL_RECT_SIZE,
                                           CW_SDL_VERT_RECTS * CW_SDL_RECT_SIZE,
                                           32,
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                                           0xff000000,
                                           0x00ff0000,
                                           0x0000ff00,
                                           0x000000ff
#else
                                           0x000000ff,
                                           0x0000ff00,
                                           0x00ff0000,
                                           0xff000000
#endif
                                          );

    if (!zeussdl.surface) {
        sdlcleanup();

        return 1;
    }

    return 0;
}

int
sdlloadbuttonimgs(struct zeussdlbuttonimgs *imgs)
{
    SDL_Rect    destrect;
    int         ret = 0;

    imgs->norm = IMG_Load("buttonnormal.png");
    imgs->hover = IMG_Load("buttonhilite.png");
    imgs->click = IMG_Load("buttonpress.png");
    if (!imgs->norm
        || !imgs->hover
        || !imgs->click) {
        SDL_DestroyTexture(imgs->normtexture);
        SDL_DestroyTexture(imgs->hovertexture);
        SDL_DestroyTexture(imgs->clicktexture);

        return 1;
    }
    imgs->rend = SDL_CreateRenderer(zeussdl.gui,
                                    -1,
                                    SDL_RENDERER_ACCELERATED
                                    | SDL_RENDERER_PRESENTVSYNC);
    destrect.x = 0;
    destrect.y = 0;
    destrect.w = CW_SDL_BUTTON_WIDTH;
    destrect.h = CW_SDL_BUTTON_HEIGHT;
    imgs->normtexture = SDL_CreateTextureFromSurface(imgs->rend,
                                                     imgs->norm);
    imgs->hovertexture = SDL_CreateTextureFromSurface(imgs->rend,
                                                      imgs->hover);
    imgs->clicktexture = SDL_CreateTextureFromSurface(imgs->rend,
                                                      imgs->click);
    SDL_RenderCopy(imgs->rend, imgs->normtexture, NULL, &destrect);
    destrect.y += CW_SDL_BUTTON_HEIGHT;
    SDL_RenderCopy(imgs->rend, imgs->normtexture, NULL, &destrect);
    destrect.y += CW_SDL_BUTTON_HEIGHT;
    SDL_RenderCopy(imgs->rend, imgs->normtexture, NULL, &destrect);
    SDL_RenderPresent(imgs->rend);

    return ret;
}

int
sdlsetbgimg(void)
{
    if (zeussdl.win) {
        zeussdl.rend = SDL_CreateRenderer(zeussdl.win,
                                          -1,
                                          SDL_RENDERER_ACCELERATED
                                          | SDL_RENDERER_PRESENTVSYNC);
        if (zeussdl.rend) {
            zeussdl.bmp = SDL_LoadBMP(SDL_BACKGROUND_IMAGE);
            if (zeussdl.bmp) {
                zeussdl.texture = SDL_CreateTextureFromSurface(zeussdl.rend,
                                                               zeussdl.bmp);
                if (zeussdl.texture) {
                    SDL_RenderClear(zeussdl.rend);
                    SDL_RenderCopy(zeussdl.rend, zeussdl.texture, NULL, NULL);
                    SDL_RenderPresent(zeussdl.rend);

                    return 0;
                }
            }
        }
    }

    return 1;
}

long
sdlinit(struct zeussdl *zeus)
{
    SDL_Init(SDL_INIT_VIDEO);
    //    SDL_WM_SetCaption("Core War Zeus", NULL);
    //    SDL_SetVideoMode(800, 600, 32, SDL_PREALLOC);
    if (sdlmksurface()) {
        sdlcleanup();

        return 1;
    }
    zeus->win = SDL_CreateWindow("Zeus",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 CW_SDL_WIDTH, CW_SDL_HEIGHT,
                                 SDL_WINDOW_SHOWN);
    if (zeus->win) {
        zeus->gui = SDL_CreateWindow("Zeus GUI",
                                     100 + CW_SDL_WIDTH, 100,
                                     CW_SDL_BUTTON_WIDTH,
                                     CW_SDL_GUI_BUTTONS * CW_SDL_BUTTON_HEIGHT,
                                     SDL_WINDOW_SHOWN);
        if (zeus->gui) {
            if (!sdlsetbgimg()) {
                zeussimwinid = SDL_GetWindowID(zeus->win);
                zeusguiwinid = SDL_GetWindowID(zeus->gui);
                if (!sdlloadbuttonimgs(&zeussdlbuttonimgs)) {
                    sdlinitrects(zeussdlrects);

                    return 0;
                }
            }
        }
    }
    sdlcleanup();

    return 1;
}

void
sdlevloop(void)
{
    zeussdlevfunc  *func;
    SDL_Event       event;
    int             loop = 1;

    while (loop) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                    if (event.window.windowID == zeussimwinid) {
                        SDL_RenderPresent(zeussdl.rend);
                    } else {
                        SDL_RenderPresent(zeussdlbuttonimgs.rend);
                    }
                }

                break;
            case SDL_KEYDOWN:
                if (event.key.windowID == zeussimwinid) {
                    if (event.key.keysym.sym == SDLK_q) {
                        loop = 0;
                    } else if (event.key.keysym.sym == SDLK_r) {
                        zeusrun(&zeussdl, &event);
                    } else if (event.key.keysym.sym == SDLK_s) {
                        zeusstop(&zeussdl, &event);
                    } else if (event.key.keysym.sym == SDLK_SPACE) {
                        zeusstep(&zeussdl, &event);
                    }
                }

                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.windowID == zeusguiwinid) {
                    Uint16 y = event.button.y;

                    y /= CW_SDL_BUTTON_HEIGHT;
                    func = zeussdlguiwins[y].func;
                    func(&zeussdl, &event);
                }

                break;
            case SDL_QUIT:
                loop = 0;

                break;
            default:

                break;
        }
    }
    SDL_Quit();

    return;
}

void
zeusdrawsimop(struct zeussdl *sdl, long pc, long sync)
{
    struct cwinstr *op;
    SDL_Rect       *rect;
    long            pid;
    int             row = pc / CW_SDL_HORIZ_RECTS;
    int             col = pc - row * CW_SDL_VERT_RECTS;
    int             x = col * CW_SDL_RECT_SIZE;
    int             y = row * CW_SDL_RECT_SIZE;

    rect = &zeussdlrects[CW_SDL_HORIZ_RECTS * y + x];
    op = &g_cwmars.core[pc];
    pid = op->pid;
    if ((g_zeussel.bmap) && bitset(g_zeussel.bmap, pc)) {
        SDL_FillRect(zeussdl.surface, rect,
                     SDL_MapRGB(zeussdl.surface->format, 255, 255, 255));
    } else if (op->op == CW_OP_DAT) {
        if (!op->red) {
            SDL_FillRect(zeussdl.surface, rect,
                         SDL_MapRGB(zeussdl.surface->format, 255, 0, 255));
        } else if (pid) {
            SDL_FillRect(zeussdl.surface, rect,
                         SDL_MapRGB(zeussdl.surface->format, 255, 140, 0));
        } else {
            SDL_FillRect(zeussdl.surface, rect,
                         SDL_MapRGB(zeussdl.surface->format, 0, 100, 0));
        }
    } else if (pid) {
        SDL_FillRect(zeussdl.surface, rect,
                     SDL_MapRGB(zeussdl.surface->format, 255, 165, 0));
    } else {
        SDL_FillRect(zeussdl.surface, rect,
                     SDL_MapRGB(zeussdl.surface->format, 0, 255, 0));
    }
    if (sync) {
        zeussdl.texture = SDL_CreateTextureFromSurface(zeussdl.rend,
                                                       zeussdl.surface);
        //    SDL_RenderClear(zeussdl.rend);
        SDL_RenderCopy(zeussdl.rend, zeussdl.texture, rect, rect);
        SDL_RenderPresent(zeussdl.rend);
    }

    return;
}

void
zeusdrawsim(struct zeussdl *sdl)
{
    long        pc;
    int         simw = CW_SDL_HORIZ_RECTS * CW_SDL_RECT_SIZE;
    int         simh = CW_SDL_VERT_RECTS * CW_SDL_RECT_SIZE;
    int         simx = (CW_SDL_WIDTH - simw) / 2;
    int         simy = (CW_SDL_HEIGHT - simh) / 2;
    SDL_Rect    rect;

    for (pc = 0 ; pc < CW_CORE_SIZE ; pc++) {
        zeusdrawsimop(sdl, pc, 0);
    }
    rect.x = simx;
    rect.y = simy;
    rect.w = simw;
    rect.h = simh;
    zeussdl.texture = SDL_CreateTextureFromSurface(zeussdl.rend,
                                                   zeussdl.surface);
    //    SDL_RenderClear(zeussdl.rend);
    SDL_RenderCopy(zeussdl.rend, zeussdl.texture, NULL, &rect);
    SDL_RenderPresent(zeussdl.rend);

    return;
}

#if defined(TEST_SDL)
int
main(int argc, char *argv[])
{
    FILE *fp;
    long  base;
    long  lim;
    long  pc1;
    long  pc2;

    sdlinit();
    if (argc != 3) {
        fprintf(stderr, "usage: %s prog1.rc prog2.rc\n", argv[0]);

        exit(1);
    }
    cwinit();
    base = utilrand() % CW_CORE_SIZE;
    fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[1]);

        exit(1);
    }
    pc1 = rcxlate(fp, 0, base, &base, &lim);
    if (pc1 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
    base = utilrand() % CW_CORE_SIZE;
    fp = fopen(argv[2], "r");
    if (!fp) {
        fprintf(stderr, "failed to open %s\n", argv[2]);

        exit(1);
    }
    pc2 = rcxlate(fp, 1, base, &base, &lim);
    if (pc2 < 0) {
        fprintf(stderr, "failed to translate %s\n", argv[1]);

        exit(1);
    }
    fclose(fp);
    g_cwmars.proccnt[0] = 1;
    g_cwmars.proccnt[1] = 1;
    g_cwmars.curproc[0] = 0;
    g_cwmars.curproc[1] = 0;
    g_cwmars.nturn[0] = CW_MAX_TURNS;
    g_cwmars.nturn[1] = CW_MAX_TURNS;
    g_cwmars.runqtab[0][0] = pc1;
    g_cwmars.runqtab[1][0] = pc2;
    g_cwmars.curpid = utilrand() & 0x01;
    g_cwmars.prog1name = argv[1];
    g_cwmars.prog2name = argv[2];
    zeusdrawsim(&zeussdl);
    while (1) {
        sdlevloop();
    }

    /* NOTREACHED */
    exit(0);
}
#endif

#endif /* defined(ZEUS) && defined(ZEUSSDL) */

