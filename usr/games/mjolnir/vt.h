#ifndef __MJOLNIR_VT_H__
#define __MJOLNIR_VT_H__

#define VT_ATTRIBUTE_PREFIX         "\033["
#define VT_PALETTE_PREFIX           "\033]P"
/* terminal attributes */
#define VT_RESET_DEFAULTS           0
#define VT_SET_BOLD_ON              1
#define VT_SET_HALF_BRIGHT_ON       2
#define VT_SET_UNDERSCORE_ON        4
#define VT_SET_BLINK_ON             5
#define VT_SET_REVERSE_VIDEO_ON     7
#define VT_SET_NORMAL_INTENSITY_ON  22
#define VT_SET_UNDERLINE_OFF_ON     24
#define VT_SET_BLINK_OFF_ON         25
#define VT_SET_REVERSE_VIDEO_OFF    27
#define VT_SET_DEFAULT_FG_COLOR     39
#define VT_SET_DEFAULT_BG_COLOR     49
#define VT_SET_RGB_FG               38
#define VT_SET_RGB_BG               48
/* foreground colors */
#define VT_BLACK_FG                 "0;30"
#define VT_BLUE_FG                  "0;34"
#define VT_GREEN_FG                 "0;32"
#define VT_CYAN_FG                  "0;36"
#define VT_RED_FG                   "0;31"
#define VT_PURPLE_FG                "0;35"
#define VT_BROWN_FG                 "0;33"
#define VT_LIGHT_GRAY_FG            "0;37"
#define VT_DARK_GRAY_FG             "1;30"
#define VT_LIGHT_BLUE_FG            "1;34"
#define VT_LIGHT_GREEN_FG           "1;32"
#define VT_LIGHT_CYAN_FG            "1;36"
#define VT_LIGHT_RED_FG             "1;31"
#define VT_LIGHT_PURPLE_FG          "1;35"
#define VT_YELLOW_FG                "1;33"
#define VT_WHITE_FG                 "1;37"
/* background colors */
#define VT_BLACK_BG                 "0;40"
#define VT_BLUE_BG                  "0;44"
#define VT_GREEN_BG                 "0;42"
#define VT_CYAN_BG                  "0;46"
#define VT_RED_BG                   "0;41"
#define VT_PURPLE_BG                "0;45"
#define VT_BROWN_BG                 "0;43"
#define VT_LIGHT_GRAY_BG            "0;47"
#define VT_DARK_GRAY_BG             "1;40"
#define VT_LIGHT_BLUE_BG            "1;44"
#define VT_LIGHT_GREEN_BG           "1;42"
#define VT_LIGHT_CYAN_BG            "1;46"
#define VT_LIGHT_RED_BG             "1;41"
#define VT_LIGHT_PURPLE_BG          "1;45"
#define VT_YELLOW_BG                "1;43"
#define VT_WHITE_BG                 "1;47"
/* g0 character set control */
#if 0
#define VT_G0_PREFIX                "\033("
#define VT_G0_ISO_LATIN1_MAP        "B"
#define VT_G0_VT100_GRAPHICS_MAP    "0"
#define VT_G0_NULL_ROM_MAP          "U"
#define VT_G0_USER_MAP              "K"
#endif

#define VT_RESET_STRING             "\033[%dm %3d\033[m"
#define VT_SET_PARM_STRING          "\033[%dm"
#define VT_SET_G0_CHARSET_STRING    "\033(%df"
#define VT_SET_COLOR_STRING         "\033[%s"
#define VT_SET_256_COLOR_STRING     "\033[%d;5;%d"
#define VT_SET_24BIT_COLOR_STRING   "\033[%d;5;%d%d%d"
#define VT_SET_RGB_COLOR_STRING     "\033]P%x%2x%2x%2x"
#define VT_SET_80_COL_STRING        "\033[?3h"
#define VT_SET_132_COL_STRING       "\033[?3l"
#define VT_SET_WIN_TITLE_STRING     "\033]0;%sST"

struct vtparm {
    int x;
    int y;
    int w;
    int h;
};

#endif /* __MJOLNIR_VT_H__ */

