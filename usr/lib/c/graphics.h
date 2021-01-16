#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#if (defined(_MSDOS_SOURCE)                                             \
     || defined(_BORLANDC_SOURCE) || defined(_TURBOC_SOURCE))

/* graphresult error return codes */
enum graphics_errors {
    grOk             =  0,
    grNoInitGraph    = -1,
    grNotDetected    = -2,
    grFileNotFound   = -3,
    grInvalidDriver  = -4,
    grNoLoadMem      = -5,
    grNoScanMem      = -6,
    grNoFloodMem     = -7,
    grFontNotFound   = -8,
    grNoFontMem      = -9,
    grInvalidMode    = -10,
    grError          = -11,     // generic error
    grIOerror        = -12,
    grInvalidFont    = -13,
    grInvalidFontNum = -14,
    grInvalidVersion = -18
};

enum COLORS {
    BLACK        = 0,   /* dark colors */
    BLUE         = 1,
    GREEN        = 2,
    CYAN         = 3,
    RED          = 4,
    MAGENTA      = 5,
    BROWN        = 6,
    LIGHTGRAY    = 7,
    DARKGRAY     = 8,   /* light colors */
    LIGHTBLUE    = 9,
    LIGHTGREEN   = 10,
    LIGHTCYAN    = 11,
    LIGHTRED     = 12,
    LIGHTMAGENTA = 13,
    YELLOW       = 14,
    WHITE        = 15
};

enum CGA_COLORS {
    CGA_LIGHTGREEN     = 1,     /* Palette C0 Color Names   */
    CGA_LIGHTRED       = 2,
    CGA_YELLOW         = 3,

    CGA_LIGHTCYAN      = 1,     /* Palette C1 Color Names   */
    CGA_LIGHTMAGENTA   = 2,
    CGA_WHITE          = 3,

    CGA_GREEN          = 1,     /* Palette C2 Color Names   */
    CGA_RED            = 2,
    CGA_BROWN          = 3,

    CGA_CYAN           = 1,     /* Palette C3 Color Names   */
    CGA_MAGENTA        = 2,
    CGA_LIGHTGRAY      = 3
};


enum EGA_COLORS {
    EGA_BLACK        =  0,      /* dark colors */
    EGA_BLUE         =  1,
    EGA_GREEN        =  2,
    EGA_CYAN         =  3,
    EGA_RED          =  4,
    EGA_MAGENTA      =  5,
    EGA_BROWN        =  20,
    EGA_LIGHTGRAY    =  7,
    EGA_DARKGRAY     =  56,     /* light colors */
    EGA_LIGHTBLUE    =  57,
    EGA_LIGHTGREEN   =  58,
    EGA_LIGHTCYAN    =  59,
    EGA_LIGHTRED     =  60,
    EGA_LIGHTMAGENTA =  61,
    EGA_YELLOW       =  62,
    EGA_WHITE        =  63
};

enum line_styles {      /* Line styles for get/setlinestyle */
    SOLID_LINE   = 0,
    DOTTED_LINE  = 1,
    CENTER_LINE  = 2,
    DASHED_LINE  = 3,
    USERBIT_LINE = 4,   /* User defined line style */
};

enum line_widths {      /* Line widths for get/setlinestyle */
    NORM_WIDTH  = 1,
    THICK_WIDTH = 3,
};

enum font_names {
    DEFAULT_FONT        = 0,    /* 8x8 bit mapped font */
    TRIPLEX_FONT        = 1,    /* "Stroked" fonts */
    SMALL_FONT          = 2,
    SANS_SERIF_FONT     = 3,
    GOTHIC_FONT         = 4,
    SCRIPT_FONT         = 5,        
    SIMPLEX_FONT        = 6,       
    TRIPLEX_SCR_FONT    = 7,
    COMPLEX_FONT        = 8,       
    EUROPEAN_FONT       = 9,  
    BOLD_FONT           = 10      
};

#define HORIZ_DIR   0   /* left to right */
#define VERT_DIR    1   /* bottom to top */

#define USER_CHAR_SIZE  0   /* user-defined char size */

enum fill_patterns {    /* Fill patterns for get/setfillstyle */
    EMPTY_FILL,         /* fills area in background color */
    SOLID_FILL,         /* fills area in solid fill color */
    LINE_FILL,          /* --- fill */
    LTSLASH_FILL,       /* /// fill */
    SLASH_FILL,         /* /// fill with thick lines */
    BKSLASH_FILL,       /* \\\ fill with thick lines */
    LTBKSLASH_FILL,     /* \\\ fill */
    HATCH_FILL,         /* light hatch fill */
    XHATCH_FILL,        /* heavy cross hatch fill */
    INTERLEAVE_FILL,    /* interleaving line fill */
    WIDE_DOT_FILL,      /* Widely spaced dot fill */
    CLOSE_DOT_FILL,     /* Closely spaced dot fill */
    USER_FILL           /* user defined fill */
};

enum putimage_ops {     /* BitBlt operators for putimage */
    COPY_PUT,           /* MOV */
    XOR_PUT,            /* XOR */
    OR_PUT,             /* OR  */
    AND_PUT,            /* AND */
    NOT_PUT             /* NOT */
};

enum text_just {        /* Horizontal and vertical justification
                           for settextjustify */
    LEFT_TEXT   = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT  = 2,

    BOTTOM_TEXT = 0,
     /* CENTER_TEXT = 1,  already defined above */
    TOP_TEXT    = 2
};


#define MAXCOLORS 15

struct palettetype {
    unsigned char size;
    signed char colors[MAXCOLORS+1];
};

struct linesettingstype {
    int linestyle;
    unsigned upattern;
    int thickness;
};

struct textsettingstype {
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
};

struct fillsettingstype {
    int pattern;
    int color;
};

struct pointtype {
    int x, y;
};

struct viewporttype {
    int left, top, right, bottom;
    int clip;
};

struct arccoordstype {
    int x, y;
    int xstart, ystart, xend, yend;
};

#endif

#endif /* __GRAPHICS_H__ */

