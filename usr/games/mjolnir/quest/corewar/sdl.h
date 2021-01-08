#ifndef __MJOLNIR_SDL_H__
#define __MJOLNIR_SDL_H__

#include <corewar/conf.h>

#if defined(ZEUS) && defined(ZEUSSDL)

#include <SDL2/SDL.h>

/*
 * button text  - orange - 255, 215, 0
 * dat          - magenta - 255, 0, 255
 * prog1        - green - 0, 255, 0
 * prog1dat     - dark green - 0, 100, 0
 * prog2        - orange - 255, 165, 0
 * prog2dat     - dark orange - 255, 140, 0
 */
#define SDL_setbuttontextcolor(sdl) SDL_SetRenderDrawColor(255, 215, 0, 255)
#define SDL_setemptycolor(sdl)      SDL_SetRenderDrawColor(255, 255, 255, 255)
#define SDL_setdatcolor(sdl)        SDL_SetRenderDrawColor(255, 0, 255, 255)
#define SDL_setprog1color(sdl)      SDL_SetRenderDrawColor(0, 255, 0, 255)
#define SDL_setprog1datcolor(sdl)   SDL_SetRenderDrawColor(0, 100, 0, 255)
#define SDL_setprog2color(sdl)      SDL_SetRenderDrawColor(255, 165, 0, 255)
#define SDL_setprog2datcolor(sdl)   SDL_SetRenderDrawColor(255, 140, 0, 255)

#define CW_SDL_GUI_BUTTONS          3 // run, stop, step
#define CW_SDL_WIDTH                1280
#define CW_SDL_HEIGHT               (320 + CW_SDL_BUTTON_HEIGHT)
#define CW_SDL_BUTTON_WIDTH         128
#define CW_SDL_BUTTON_HEIGHT        32
#define CW_SDL_RECT_SIZE            4
#define CW_SDL_HORIZ_RECTS          100
#define CW_SDL_VERT_RECTS           80

struct zeussdl {
    int                 simw;
    SDL_Window         *win;
    SDL_Window         *gui;
    SDL_Renderer       *rend;
    SDL_Surface        *surface;
    SDL_Texture        *texture;
    SDL_Surface        *bmp;
};

typedef void                        zeussdlevfunc(struct zeussdl *,
                                                  SDL_Event *);

struct zeussdlguiwin {
    const char         *text;
    zeussdlevfunc      *func;
};

struct zeussdlevfuncs {
    zeussdlevfunc      *enter;
    zeussdlevfunc      *leave;
    zeussdlevfunc      *click;
    zeussdlevfunc      *release;
    zeussdlevfunc      *expose;
};

struct zeussdlbuttonimgs {
    SDL_Renderer       *rend;
    SDL_Surface        *norm;
    SDL_Texture        *normtexture;
    SDL_Surface        *hover;
    SDL_Texture        *hovertexture;
    SDL_Surface        *click;
    SDL_Texture        *clicktexture;
};

long                               sdlinit(struct zeussdl *zeus);
void                               zeusdrawsim(struct zeussdl *sdl);
void                               zeusdrawsimop(struct zeussdl *sdl,
                                                 long pc,
                                                 long sync);

#endif /* defined(ZEUS) && defined(ZEUSSDL) */

#endif /* __MJOLNIR_SDL_H__ */

