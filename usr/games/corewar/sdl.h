#ifndef __COREWAR_SDL_H__
#define __COREWAR_SDL_H__

#include <SDL2/SDL.h>

#define CWSDLGUIBUTTONS     3 // run, stop, step

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

#define CWSDLWIDTH          960
#define CWSDLHEIGHT         540
#define CWSDLBUTTONWIDTH    128
#define CWSDLBUTTONHEIGHT   32
#define CWSDLRECTSIZE       4
#define CWSDLHORIZRECTS     100
#define CWSDLVERTRECTS      80

struct zeussdl;
typedef void zeussdlevfunc(struct zeussdl *, SDL_Event *);

struct zeussdl {
    int            simw;
	SDL_Window     *win;
	SDL_Window     *gui;
	SDL_Renderer   *rend;
	SDL_Surface    *surface;
	SDL_Texture    *texture;
	SDL_Surface    *bmp;
};

struct zeussdlguiwin {
    const char     *text;
    zeussdlevfunc  *func;
};

struct zeussdlevfuncs {
    zeussdlevfunc  *enter;
    zeussdlevfunc  *leave;
    zeussdlevfunc  *click;
    zeussdlevfunc  *release;
    zeussdlevfunc  *expose;
};

struct zeussdlbuttonimgs {
    SDL_Renderer   *rend;
    SDL_Surface    *norm;
    SDL_Texture    *normtexture;
    SDL_Surface    *hover;
    SDL_Texture    *hovertexture;
    SDL_Surface    *click;
    SDL_Texture    *clicktexture;
};

#endif /* __COREWAR_SDL_H__ */

