#ifndef __VT_COLOR_H__
#define __VT_COLOR_H__

/* regular terminal colors */
#define VT_BLACK_COLOR          0x000000
#define VT_RED_COLOR            0xaa0000
#define VT_GREEN_COLOR          0x008800
#define VT_YELLOW_COLOR         0xaa5522
#define VT_BLUE_COLOR           0x0000aa
#define VT_MAGENTA_COLOR        0xaa00aa
#define VT_CYAN_COLOR           0x00aaaa
#define VT_WHITE_COLOR          0xaaaaaa
/* bright terminal colors */
#define VT_BRIGHT_BLACK_COLOR   0x808080
#define VT_BRIGHT_RED_COLOR     0xff8080
#define VT_BRIGHT_GREEN_COLOR   0x80ff80
#define VT_BRIGHT_YELLOW_COLOR  0xffff80
#define VT_BRIGHT_BLUE_COLOR    0x8080ff
#define VT_BRIGHT_MAGENTA_COLOR 0xff80ff
#define VT_BRIGHT_CYAN_COLOR    0x80ffff
#define VT_BRIGHT_WHITE_COLOR   0xffffff

#define VT_DEFAULT_COLORMAP                                             \
    {                                                                   \
        VT_BLACK_COLOR,                                                 \
        VT_RED_COLOR,                                                   \
        VT_GREEN_COLOR,                                                 \
        VT_YELLOW_COLOR,                                                \
        VT_BLUE_COLOR,                                                  \
        VT_MAGENTA_COLOR,                                               \
        VT_CYAN_COLOR,                                                  \
        VT_WHITE_COLOR,                                                 \
        VT_BRIGHT_BLACK_COLOR,                                          \
        VT_BRIGHT_RED_COLOR,                                            \
        VT_BRIGHT_GREEN_COLOR,                                          \
        VT_BRIGHT_YELLOW_COLOR,                                         \
        VT_BRIGHT_BLUE_COLOR,                                           \
        VT_BRIGHT_MAGENTA_COLOR,                                        \
        VT_BRIGHT_CYAN_COLOR,                                           \
        VT_BRIGHT_WHITE_COLOR                                           \
    }


/* 256-color xterm palette strings; "#RRGGBB" */
#define VT_XTERM_COLOR_SPEC_0   "#000000"
#define VT_XTERM_COLOR_SPEC_1   "#cd0000"
#define VT_XTERM_COLOR_SPEC_2   "#00cd00"
#define VT_XTERM_COLOR_SPEC_3   "#cdcd00"
#define VT_XTERM_COLOR_SPEC_4   "#0000ee"
#define VT_XTERM_COLOR_SPEC_5   "#cd00cd"
#define VT_XTERM_COLOR_SPEC_6   "#00cdcd"
#define VT_XTERM_COLOR_SPEC_7   "#e5e5e5"
#define VT_XTERM_COLOR_SPEC_8   "#7f7f7f"
#define VT_XTERM_COLOR_SPEC_9   "#ff0000"
#define VT_XTERM_COLOR_SPEC_10  "#00ff00"
#define VT_XTERM_COLOR_SPEC_11  "#ffff00"
#define VT_XTERM_COLOR_SPEC_12  "#5c5cff"
#define VT_XTERM_COLOR_SPEC_13  "#ff00ff"
#define VT_XTERM_COLOR_SPEC_14  "#00ffff"
#define VT_XTERM_COLOR_SPEC_15  "#ffffff"
#define VT_XTERM_COLOR_SPEC_16  "#000000"
#define VT_XTERM_COLOR_SPEC_17  "#00005f"
#define VT_XTERM_COLOR_SPEC_18  "#000087"
#define VT_XTERM_COLOR_SPEC_19  "#0000af"
#define VT_XTERM_COLOR_SPEC_20  "#0000d7"
#define VT_XTERM_COLOR_SPEC_21  "#0000ff"
#define VT_XTERM_COLOR_SPEC_22  "#005f00"
#define VT_XTERM_COLOR_SPEC_23  "#005f5f"
#define VT_XTERM_COLOR_SPEC_24  "#005f87"
#define VT_XTERM_COLOR_SPEC_25  "#005faf"
#define VT_XTERM_COLOR_SPEC_26  "#005fd7"
#define VT_XTERM_COLOR_SPEC_27  "#005fff"
#define VT_XTERM_COLOR_SPEC_28  "#008700"
#define VT_XTERM_COLOR_SPEC_29  "#00875f"
#define VT_XTERM_COLOR_SPEC_30  "#008787"
#define VT_XTERM_COLOR_SPEC_31  "#0087af"
#define VT_XTERM_COLOR_SPEC_32  "#0087d7"
#define VT_XTERM_COLOR_SPEC_33  "#0087ff"
#define VT_XTERM_COLOR_SPEC_34  "#00af00"
#define VT_XTERM_COLOR_SPEC_35  "#00af5f"
#define VT_XTERM_COLOR_SPEC_36  "#00af87"
#define VT_XTERM_COLOR_SPEC_37  "#00afaf"
#define VT_XTERM_COLOR_SPEC_38  "#00afd7"
#define VT_XTERM_COLOR_SPEC_39  "#00afff"
#define VT_XTERM_COLOR_SPEC_40  "#00d700"
#define VT_XTERM_COLOR_SPEC_41  "#00d75f"
#define VT_XTERM_COLOR_SPEC_42  "#00d787"
#define VT_XTERM_COLOR_SPEC_43  "#00d7af"
#define VT_XTERM_COLOR_SPEC_44  "#00d7d7"
#define VT_XTERM_COLOR_SPEC_45  "#00d7ff"
#define VT_XTERM_COLOR_SPEC_46  "#00ff00"
#define VT_XTERM_COLOR_SPEC_47  "#00ff5f"
#define VT_XTERM_COLOR_SPEC_48  "#00ff87"
#define VT_XTERM_COLOR_SPEC_49  "#00ffaf"
#define VT_XTERM_COLOR_SPEC_50  "#00ffd7"
#define VT_XTERM_COLOR_SPEC_51  "#00ffff"
#define VT_XTERM_COLOR_SPEC_52  "#5f0000"
#define VT_XTERM_COLOR_SPEC_53  "#5f005f"
#define VT_XTERM_COLOR_SPEC_54  "#5f0087"
#define VT_XTERM_COLOR_SPEC_55  "#5f00af"
#define VT_XTERM_COLOR_SPEC_56  "#5f00d7"
#define VT_XTERM_COLOR_SPEC_57  "#5f00ff"
#define VT_XTERM_COLOR_SPEC_58  "#5f5f00"
#define VT_XTERM_COLOR_SPEC_59  "#5f5f5f"
#define VT_XTERM_COLOR_SPEC_60  "#5f5f87"
#define VT_XTERM_COLOR_SPEC_61  "#5f5faf"
#define VT_XTERM_COLOR_SPEC_62  "#5f5fd7"
#define VT_XTERM_COLOR_SPEC_63  "#5f5fff"
#define VT_XTERM_COLOR_SPEC_64  "#5f8700"
#define VT_XTERM_COLOR_SPEC_65  "#5f875f"
#define VT_XTERM_COLOR_SPEC_66  "#5f8787"
#define VT_XTERM_COLOR_SPEC_67  "#5f87af"
#define VT_XTERM_COLOR_SPEC_68  "#5f87d7"
#define VT_XTERM_COLOR_SPEC_69  "#5f87ff"
#define VT_XTERM_COLOR_SPEC_70  "#5faf00"
#define VT_XTERM_COLOR_SPEC_71  "#5faf5f"
#define VT_XTERM_COLOR_SPEC_72  "#5faf87"
#define VT_XTERM_COLOR_SPEC_73  "#5fafaf"
#define VT_XTERM_COLOR_SPEC_74  "#5fafd7"
#define VT_XTERM_COLOR_SPEC_75  "#5fafff"
#define VT_XTERM_COLOR_SPEC_76  "#5fd700"
#define VT_XTERM_COLOR_SPEC_77  "#5fd75f"
#define VT_XTERM_COLOR_SPEC_78  "#5fd787"
#define VT_XTERM_COLOR_SPEC_79  "#5fd7af"
#define VT_XTERM_COLOR_SPEC_80  "#5fd7d7"
#define VT_XTERM_COLOR_SPEC_81  "#5fffff"
#define VT_XTERM_COLOR_SPEC_82  "#5fff00"
#define VT_XTERM_COLOR_SPEC_83  "#5fff5f"
#define VT_XTERM_COLOR_SPEC_84  "#5fff87"
#define VT_XTERM_COLOR_SPEC_85  "#5fffaf"
#define VT_XTERM_COLOR_SPEC_86  "#5fffd7"
#define VT_XTERM_COLOR_SPEC_87  "#5fffff"
#define VT_XTERM_COLOR_SPEC_88  "#870000"
#define VT_XTERM_COLOR_SPEC_89  "#87005f"
#define VT_XTERM_COLOR_SPEC_90  "#870087"
#define VT_XTERM_COLOR_SPEC_91  "#8700af"
#define VT_XTERM_COLOR_SPEC_92  "#8700d7"
#define VT_XTERM_COLOR_SPEC_93  "#8700ff"
#define VT_XTERM_COLOR_SPEC_94  "#875f00"
#define VT_XTERM_COLOR_SPEC_95  "#875f5f"
#define VT_XTERM_COLOR_SPEC_96  "#875f87"
#define VT_XTERM_COLOR_SPEC_97  "#875faf"
#define VT_XTERM_COLOR_SPEC_98  "#875fd7"
#define VT_XTERM_COLOR_SPEC_99  "#875fff"
#define VT_XTERM_COLOR_SPEC_100 "#878700"
#define VT_XTERM_COLOR_SPEC_101 "#87875f"
#define VT_XTERM_COLOR_SPEC_102 "#878787"
#define VT_XTERM_COLOR_SPEC_103 "#8787af"
#define VT_XTERM_COLOR_SPEC_104 "#8787d7"
#define VT_XTERM_COLOR_SPEC_105 "#8787ff"
#define VT_XTERM_COLOR_SPEC_106 "#87af00"
#define VT_XTERM_COLOR_SPEC_107 "#87af5f"
#define VT_XTERM_COLOR_SPEC_108 "#87af87"
#define VT_XTERM_COLOR_SPEC_109 "#87afaf"
#define VT_XTERM_COLOR_SPEC_110 "#87afd7"
#define VT_XTERM_COLOR_SPEC_111 "#87afff"
#define VT_XTERM_COLOR_SPEC_112 "#87d700"
#define VT_XTERM_COLOR_SPEC_113 "#87d75f"
#define VT_XTERM_COLOR_SPEC_114 "#87d787"
#define VT_XTERM_COLOR_SPEC_115 "#87d7af"
#define VT_XTERM_COLOR_SPEC_116 "#87d7d7"
#define VT_XTERM_COLOR_SPEC_117 "#87d7ff"
#define VT_XTERM_COLOR_SPEC_118 "#87ff00"
#define VT_XTERM_COLOR_SPEC_119 "#87ff5f"
#define VT_XTERM_COLOR_SPEC_120 "#87ff87"
#define VT_XTERM_COLOR_SPEC_121 "#87ffaf"
#define VT_XTERM_COLOR_SPEC_122 "#87ffd7"
#define VT_XTERM_COLOR_SPEC_123 "#87ffff"
#define VT_XTERM_COLOR_SPEC_124 "#af0000"
#define VT_XTERM_COLOR_SPEC_125 "#af005f"
#define VT_XTERM_COLOR_SPEC_126 "#af0087"
#define VT_XTERM_COLOR_SPEC_127 "#af00af"
#define VT_XTERM_COLOR_SPEC_128 "#af00d7"
#define VT_XTERM_COLOR_SPEC_129 "#af00ff"
#define VT_XTERM_COLOR_SPEC_130 "#af5f00" 
#define VT_XTERM_COLOR_SPEC_131 "#af5f5f"
#define VT_XTERM_COLOR_SPEC_132 "#af5f87"
#define VT_XTERM_COLOR_SPEC_133 "#af5faf"
#define VT_XTERM_COLOR_SPEC_134 "#af5fd7"
#define VT_XTERM_COLOR_SPEC_135 "#af5fff"
#define VT_XTERM_COLOR_SPEC_136 "#af8700"
#define VT_XTERM_COLOR_SPEC_137 "#af875f"
#define VT_XTERM_COLOR_SPEC_138 "#af8787"
#define VT_XTERM_COLOR_SPEC_139 "#af87af"
#define VT_XTERM_COLOR_SPEC_140 "#af87d7"
#define VT_XTERM_COLOR_SPEC_141 "#af87ff"
#define VT_XTERM_COLOR_SPEC_142 "#afaf00"
#define VT_XTERM_COLOR_SPEC_143 "#afaf5f"
#define VT_XTERM_COLOR_SPEC_144 "#afaf87"
#define VT_XTERM_COLOR_SPEC_145 "#afafaf"
#define VT_XTERM_COLOR_SPEC_146 "#afafd7"
#define VT_XTERM_COLOR_SPEC_147 "#afafff"
#define VT_XTERM_COLOR_SPEC_148 "#afd700"
#define VT_XTERM_COLOR_SPEC_149 "#afd75f"
#define VT_XTERM_COLOR_SPEC_150 "#afd787"
#define VT_XTERM_COLOR_SPEC_151 "#afd7af"
#define VT_XTERM_COLOR_SPEC_152 "#afd7d7"
#define VT_XTERM_COLOR_SPEC_153 "#afd7ff"
#define VT_XTERM_COLOR_SPEC_154 "#afff00"
#define VT_XTERM_COLOR_SPEC_155 "#afff5f"
#define VT_XTERM_COLOR_SPEC_156 "#afff87"
#define VT_XTERM_COLOR_SPEC_157 "#afffaf"
#define VT_XTERM_COLOR_SPEC_158 "#afffd7"
#define VT_XTERM_COLOR_SPEC_159 "#afffff"
#define VT_XTERM_COLOR_SPEC_160 "#d70000"
#define VT_XTERM_COLOR_SPEC_161 "#d7005f"
#define VT_XTERM_COLOR_SPEC_162 "#d70087"
#define VT_XTERM_COLOR_SPEC_163 "#d700af"
#define VT_XTERM_COLOR_SPEC_164 "#d700d7"
#define VT_XTERM_COLOR_SPEC_165 "#d700ff"
#define VT_XTERM_COLOR_SPEC_166 "#d75f00"
#define VT_XTERM_COLOR_SPEC_167 "#d75f5f"
#define VT_XTERM_COLOR_SPEC_168 "#d75f87"
#define VT_XTERM_COLOR_SPEC_169 "#d75faf"
#define VT_XTERM_COLOR_SPEC_170 "#d75fd7"
#define VT_XTERM_COLOR_SPEC_171 "#d75fff"
#define VT_XTERM_COLOR_SPEC_172 "#d78700"
#define VT_XTERM_COLOR_SPEC_173 "#d7875f"
#define VT_XTERM_COLOR_SPEC_174 "#d78787"
#define VT_XTERM_COLOR_SPEC_175 "#d787af"
#define VT_XTERM_COLOR_SPEC_176 "#d787d7"
#define VT_XTERM_COLOR_SPEC_177 "#d787ff"
#define VT_XTERM_COLOR_SPEC_178 "#d7af00"
#define VT_XTERM_COLOR_SPEC_179 "#d7af5f"
#define VT_XTERM_COLOR_SPEC_180 "#d7af87"
#define VT_XTERM_COLOR_SPEC_181 "#d7afaf"
#define VT_XTERM_COLOR_SPEC_182 "#d7afd7"
#define VT_XTERM_COLOR_SPEC_183 "#d7afff"
#define VT_XTERM_COLOR_SPEC_184 "#d7d700"
#define VT_XTERM_COLOR_SPEC_185 "#d7d75f"
#define VT_XTERM_COLOR_SPEC_186 "#d7d787"
#define VT_XTERM_COLOR_SPEC_187 "#d7d7af"
#define VT_XTERM_COLOR_SPEC_188 "#d7d7d7"
#define VT_XTERM_COLOR_SPEC_189 "#d7d7ff"
#define VT_XTERM_COLOR_SPEC_190 "#d7ff00"
#define VT_XTERM_COLOR_SPEC_191 "#d7ff5f"
#define VT_XTERM_COLOR_SPEC_192 "#d7ff87"
#define VT_XTERM_COLOR_SPEC_193 "#d7ffaf"
#define VT_XTERM_COLOR_SPEC_194 "#d7ffd7"
#define VT_XTERM_COLOR_SPEC_195 "#d7ffff"
#define VT_XTERM_COLOR_SPEC_196 "#ff0000"
#define VT_XTERM_COLOR_SPEC_197 "#ff005f"
#define VT_XTERM_COLOR_SPEC_198 "#ff0087"
#define VT_XTERM_COLOR_SPEC_199 "#ff00af"
#define VT_XTERM_COLOR_SPEC_200 "#ff00d7"
#define VT_XTERM_COLOR_SPEC_201 "#ff00ff"
#define VT_XTERM_COLOR_SPEC_202 "#ff5f00"
#define VT_XTERM_COLOR_SPEC_203 "#ff5f5f"
#define VT_XTERM_COLOR_SPEC_204 "#ff5f87"
#define VT_XTERM_COLOR_SPEC_205 "#ff5faf"
#define VT_XTERM_COLOR_SPEC_206 "#ff5fd7"
#define VT_XTERM_COLOR_SPEC_207 "#ff5fff"
#define VT_XTERM_COLOR_SPEC_208 "#ff8700"
#define VT_XTERM_COLOR_SPEC_209 "#ff875f"
#define VT_XTERM_COLOR_SPEC_210 "#ff8787"
#define VT_XTERM_COLOR_SPEC_211 "#ff87af"
#define VT_XTERM_COLOR_SPEC_212 "#ff87d7"
#define VT_XTERM_COLOR_SPEC_213 "#ff87ff"
#define VT_XTERM_COLOR_SPEC_214 "#ffaf00"
#define VT_XTERM_COLOR_SPEC_215 "#ffaf5f"
#define VT_XTERM_COLOR_SPEC_216 "#ffaf87"
#define VT_XTERM_COLOR_SPEC_217 "#ffafaf"
#define VT_XTERM_COLOR_SPEC_218 "#ffafd7"
#define VT_XTERM_COLOR_SPEC_219 "#ffafff"
#define VT_XTERM_COLOR_SPEC_220 "#ffd700"
#define VT_XTERM_COLOR_SPEC_221 "#ffd75f"
#define VT_XTERM_COLOR_SPEC_222 "#ffd787"
#define VT_XTERM_COLOR_SPEC_223 "#ffd7af"
#define VT_XTERM_COLOR_SPEC_224 "#ffd7d7"
#define VT_XTERM_COLOR_SPEC_225 "#ffd7ff"
#define VT_XTERM_COLOR_SPEC_226 "#ffff00"
#define VT_XTERM_COLOR_SPEC_227 "#ffff5f"
#define VT_XTERM_COLOR_SPEC_228 "#ffff87"
#define VT_XTERM_COLOR_SPEC_229 "#ffffaf"
#define VT_XTERM_COLOR_SPEC_230 "#ffffd7"
#define VT_XTERM_COLOR_SPEC_231 "#ffffff"
/* gray shades */
#define VT_XTERM_COLOR_SPEC_232 "#000000"
#define VT_XTERM_COLOR_SPEC_233 "#121212"
#define VT_XTERM_COLOR_SPEC_234 "#1c1c1c"
#define VT_XTERM_COLOR_SPEC_235 "#262626"
#define VT_XTERM_COLOR_SPEC_236 "#303030"
#define VT_XTERM_COLOR_SPEC_237 "#3a3a3a"
#define VT_XTERM_COLOR_SPEC_238 "#444444"
#define VT_XTERM_COLOR_SPEC_239 "#4e4e4e"
#define VT_XTERM_COLOR_SPEC_240 "#585858"
#define VT_XTERM_COLOR_SPEC_241 "#626262"
#define VT_XTERM_COLOR_SPEC_242 "#6c6c6c"
#define VT_XTERM_COLOR_SPEC_243 "#767676"
#define VT_XTERM_COLOR_SPEC_244 "#808080"
#define VT_XTERM_COLOR_SPEC_245 "#8a8a8a"
#define VT_XTERM_COLOR_SPEC_246 "#949494"
#define VT_XTERM_COLOR_SPEC_247 "#9e9e9e"
#define VT_XTERM_COLOR_SPEC_248 "#a8a8a8"
#define VT_XTERM_COLOR_SPEC_249 "#b2b2b2"
#define VT_XTERM_COLOR_SPEC_250 "#bcbcbc"
#define VT_XTERM_COLOR_SPEC_251 "#c6c6c6"
#define VT_XTERM_COLOR_SPEC_252 "#d0d0d0"
#define VT_XTERM_COLOR_SPEC_253 "#dadada"
#define VT_XTERM_COLOR_SPEC_254 "#e4e4e4"
#define VT_XTERM_COLOR_SPEC_255 "#eeeeee"

/* 256-color xterm palette color values; 0xRRGGBB */
#define VT_XTERM_COLOR_0        0x000000
#define VT_XTERM_COLOR_1        0xcd0000
#define VT_XTERM_COLOR_2        0x00cd00
#define VT_XTERM_COLOR_3        0xcdcd00
#define VT_XTERM_COLOR_4        0x0000ee
#define VT_XTERM_COLOR_5        0xcd00cd
#define VT_XTERM_COLOR_6        0x00cdcd
#define VT_XTERM_COLOR_7        0xe5e5e5
#define VT_XTERM_COLOR_8        0x7f7f7f
#define VT_XTERM_COLOR_9        0xff0000
#define VT_XTERM_COLOR_10       0x00ff00
#define VT_XTERM_COLOR_11       0xffff00
#define VT_XTERM_COLOR_12       0x5c5cff
#define VT_XTERM_COLOR_13       0xff00ff
#define VT_XTERM_COLOR_14       0x00ffff
#define VT_XTERM_COLOR_15       0xffffff
#define VT_XTERM_COLOR_16       0x000000
#define VT_XTERM_COLOR_17       0x00005f
#define VT_XTERM_COLOR_18       0x000087
#define VT_XTERM_COLOR_19       0x0000af
#define VT_XTERM_COLOR_20       0x0000d7
#define VT_XTERM_COLOR_21       0x0000ff
#define VT_XTERM_COLOR_22       0x005f00
#define VT_XTERM_COLOR_23       0x005f5f
#define VT_XTERM_COLOR_24       0x005f87
#define VT_XTERM_COLOR_25       0x005faf
#define VT_XTERM_COLOR_26       0x005fd7
#define VT_XTERM_COLOR_27       0x005fff
#define VT_XTERM_COLOR_28       0x008700
#define VT_XTERM_COLOR_29       0x00875f
#define VT_XTERM_COLOR_30       0x008787
#define VT_XTERM_COLOR_31       0x0087af
#define VT_XTERM_COLOR_32       0x0087d7
#define VT_XTERM_COLOR_33       0x0087ff
#define VT_XTERM_COLOR_34       0x00af00
#define VT_XTERM_COLOR_35       0x00af5f
#define VT_XTERM_COLOR_36       0x00af87
#define VT_XTERM_COLOR_37       0x00afaf
#define VT_XTERM_COLOR_38       0x00afd7
#define VT_XTERM_COLOR_39       0x00afff
#define VT_XTERM_COLOR_40       0x00d700
#define VT_XTERM_COLOR_41       0x00d75f
#define VT_XTERM_COLOR_42       0x00d787
#define VT_XTERM_COLOR_43       0x00d7af
#define VT_XTERM_COLOR_44       0x00d7d7
#define VT_XTERM_COLOR_45       0x00d7ff
#define VT_XTERM_COLOR_46       0x00ff00
#define VT_XTERM_COLOR_47       0x00ff5f
#define VT_XTERM_COLOR_48       0x00ff87
#define VT_XTERM_COLOR_49       0x00ffaf
#define VT_XTERM_COLOR_50       0x00ffd7
#define VT_XTERM_COLOR_51       0x00ffff
#define VT_XTERM_COLOR_52       0x5f0000
#define VT_XTERM_COLOR_53       0x5f005f
#define VT_XTERM_COLOR_54       0x5f0087
#define VT_XTERM_COLOR_55       0x5f00af
#define VT_XTERM_COLOR_56       0x5f00d7
#define VT_XTERM_COLOR_57       0x5f00ff
#define VT_XTERM_COLOR_58       0x5f5f00
#define VT_XTERM_COLOR_59       0x5f5f5f
#define VT_XTERM_COLOR_60       0x5f5f87
#define VT_XTERM_COLOR_61       0x5f5faf
#define VT_XTERM_COLOR_62       0x5f5fd7
#define VT_XTERM_COLOR_63       0x5f5fff
#define VT_XTERM_COLOR_64       0x5f8700
#define VT_XTERM_COLOR_65       0x5f875f
#define VT_XTERM_COLOR_66       0x5f8787
#define VT_XTERM_COLOR_67       0x5f87af
#define VT_XTERM_COLOR_68       0x5f87d7
#define VT_XTERM_COLOR_69       0x5f87ff
#define VT_XTERM_COLOR_70       0x5faf00
#define VT_XTERM_COLOR_71       0x5faf5f
#define VT_XTERM_COLOR_72       0x5faf87
#define VT_XTERM_COLOR_73       0x5fafaf
#define VT_XTERM_COLOR_74       0x5fafd7
#define VT_XTERM_COLOR_75       0x5fafff
#define VT_XTERM_COLOR_76       0x5fd700
#define VT_XTERM_COLOR_77       0x5fd75f
#define VT_XTERM_COLOR_78       0x5fd787
#define VT_XTERM_COLOR_79       0x5fd7af
#define VT_XTERM_COLOR_80       0x5fd7d7
#define VT_XTERM_COLOR_81       0x5fffff
#define VT_XTERM_COLOR_82       0x5fff00
#define VT_XTERM_COLOR_83       0x5fff5f
#define VT_XTERM_COLOR_84       0x5fff87
#define VT_XTERM_COLOR_85       0x5fffaf
#define VT_XTERM_COLOR_86       0x5fffd7
#define VT_XTERM_COLOR_87       0x5fffff
#define VT_XTERM_COLOR_88       0x870000
#define VT_XTERM_COLOR_89       0x87005f
#define VT_XTERM_COLOR_90       0x870087
#define VT_XTERM_COLOR_91       0x8700af
#define VT_XTERM_COLOR_92       0x8700d7
#define VT_XTERM_COLOR_93       0x8700ff
#define VT_XTERM_COLOR_94       0x875f00
#define VT_XTERM_COLOR_95       0x875f5f
#define VT_XTERM_COLOR_96       0x875f87
#define VT_XTERM_COLOR_97       0x875faf
#define VT_XTERM_COLOR_98       0x875fd7
#define VT_XTERM_COLOR_99       0x875fff
#define VT_XTERM_COLOR_100      0x878700
#define VT_XTERM_COLOR_101      0x87875f
#define VT_XTERM_COLOR_102      0x878787
#define VT_XTERM_COLOR_103      0x8787af
#define VT_XTERM_COLOR_104      0x8787d7
#define VT_XTERM_COLOR_105      0x8787ff
#define VT_XTERM_COLOR_106      0x87af00
#define VT_XTERM_COLOR_107      0x87af5f
#define VT_XTERM_COLOR_108      0x87af87
#define VT_XTERM_COLOR_109      0x87afaf
#define VT_XTERM_COLOR_110      0x87afd7
#define VT_XTERM_COLOR_111      0x87afff
#define VT_XTERM_COLOR_112      0x87d700
#define VT_XTERM_COLOR_113      0x87d75f
#define VT_XTERM_COLOR_114      0x87d787
#define VT_XTERM_COLOR_115      0x87d7af
#define VT_XTERM_COLOR_116      0x87d7d7
#define VT_XTERM_COLOR_117      0x87d7ff
#define VT_XTERM_COLOR_118      0x87ff00
#define VT_XTERM_COLOR_119      0x87ff5f
#define VT_XTERM_COLOR_120      0x87ff87
#define VT_XTERM_COLOR_121      0x87ffaf
#define VT_XTERM_COLOR_122      0x87ffd7
#define VT_XTERM_COLOR_123      0x87ffff
#define VT_XTERM_COLOR_124      0xaf0000
#define VT_XTERM_COLOR_125      0xaf005f
#define VT_XTERM_COLOR_126      0xaf0087
#define VT_XTERM_COLOR_127      0xaf00af
#define VT_XTERM_COLOR_128      0xaf00d7
#define VT_XTERM_COLOR_129      0xaf00ff
#define VT_XTERM_COLOR_130      0xaf5f00 
#define VT_XTERM_COLOR_131      0xaf5f5f
#define VT_XTERM_COLOR_132      0xaf5f87
#define VT_XTERM_COLOR_133      0xaf5faf
#define VT_XTERM_COLOR_134      0xaf5fd7
#define VT_XTERM_COLOR_135      0xaf5fff
#define VT_XTERM_COLOR_136      0xaf8700
#define VT_XTERM_COLOR_137      0xaf875f
#define VT_XTERM_COLOR_138      0xaf8787
#define VT_XTERM_COLOR_139      0xaf87af
#define VT_XTERM_COLOR_140      0xaf87d7
#define VT_XTERM_COLOR_141      0xaf87ff
#define VT_XTERM_COLOR_142      0xafaf00
#define VT_XTERM_COLOR_143      0xafaf5f
#define VT_XTERM_COLOR_144      0xafaf87
#define VT_XTERM_COLOR_145      0xafafaf
#define VT_XTERM_COLOR_146      0xafafd7
#define VT_XTERM_COLOR_147      0xafafff
#define VT_XTERM_COLOR_148      0xafd700
#define VT_XTERM_COLOR_149      0xafd75f
#define VT_XTERM_COLOR_150      0xafd787
#define VT_XTERM_COLOR_151      0xafd7af
#define VT_XTERM_COLOR_152      0xafd7d7
#define VT_XTERM_COLOR_153      0xafd7ff
#define VT_XTERM_COLOR_154      0xafff00
#define VT_XTERM_COLOR_155      0xafff5f
#define VT_XTERM_COLOR_156      0xafff87
#define VT_XTERM_COLOR_157      0xafffaf
#define VT_XTERM_COLOR_158      0xafffd7
#define VT_XTERM_COLOR_159      0xafffff
#define VT_XTERM_COLOR_160      0xd70000
#define VT_XTERM_COLOR_161      0xd7005f
#define VT_XTERM_COLOR_162      0xd70087
#define VT_XTERM_COLOR_163      0xd700af
#define VT_XTERM_COLOR_164      0xd700d7
#define VT_XTERM_COLOR_165      0xd700ff
#define VT_XTERM_COLOR_166      0xd75f00
#define VT_XTERM_COLOR_167      0xd75f5f
#define VT_XTERM_COLOR_168      0xd75f87
#define VT_XTERM_COLOR_169      0xd75faf
#define VT_XTERM_COLOR_170      0xd75fd7
#define VT_XTERM_COLOR_171      0xd75fff
#define VT_XTERM_COLOR_172      0xd78700
#define VT_XTERM_COLOR_173      0xd7875f
#define VT_XTERM_COLOR_174      0xd78787
#define VT_XTERM_COLOR_175      0xd787af
#define VT_XTERM_COLOR_176      0xd787d7
#define VT_XTERM_COLOR_177      0xd787ff
#define VT_XTERM_COLOR_178      0xd7af00
#define VT_XTERM_COLOR_179      0xd7af5f
#define VT_XTERM_COLOR_180      0xd7af87
#define VT_XTERM_COLOR_181      0xd7afaf
#define VT_XTERM_COLOR_182      0xd7afd7
#define VT_XTERM_COLOR_183      0xd7afff
#define VT_XTERM_COLOR_184      0xd7d700
#define VT_XTERM_COLOR_185      0xd7d75f
#define VT_XTERM_COLOR_186      0xd7d787
#define VT_XTERM_COLOR_187      0xd7d7af
#define VT_XTERM_COLOR_188      0xd7d7d7
#define VT_XTERM_COLOR_189      0xd7d7ff
#define VT_XTERM_COLOR_190      0xd7ff00
#define VT_XTERM_COLOR_191      0xd7ff5f
#define VT_XTERM_COLOR_192      0xd7ff87
#define VT_XTERM_COLOR_193      0xd7ffaf
#define VT_XTERM_COLOR_194      0xd7ffd7
#define VT_XTERM_COLOR_195      0xd7ffff
#define VT_XTERM_COLOR_196      0xff0000
#define VT_XTERM_COLOR_197      0xff005f
#define VT_XTERM_COLOR_198      0xff0087
#define VT_XTERM_COLOR_199      0xff00af
#define VT_XTERM_COLOR_200      0xff00d7
#define VT_XTERM_COLOR_201      0xff00ff
#define VT_XTERM_COLOR_202      0xff5f00
#define VT_XTERM_COLOR_203      0xff5f5f
#define VT_XTERM_COLOR_204      0xff5f87
#define VT_XTERM_COLOR_205      0xff5faf
#define VT_XTERM_COLOR_206      0xff5fd7
#define VT_XTERM_COLOR_207      0xff5fff
#define VT_XTERM_COLOR_208      0xff8700
#define VT_XTERM_COLOR_209      0xff875f
#define VT_XTERM_COLOR_210      0xff8787
#define VT_XTERM_COLOR_211      0xff87af
#define VT_XTERM_COLOR_212      0xff87d7
#define VT_XTERM_COLOR_213      0xff87ff
#define VT_XTERM_COLOR_214      0xffaf00
#define VT_XTERM_COLOR_215      0xffaf5f
#define VT_XTERM_COLOR_216      0xffaf87
#define VT_XTERM_COLOR_217      0xffafaf
#define VT_XTERM_COLOR_218      0xffafd7
#define VT_XTERM_COLOR_219      0xffafff
#define VT_XTERM_COLOR_220      0xffd700
#define VT_XTERM_COLOR_221      0xffd75f
#define VT_XTERM_COLOR_222      0xffd787
#define VT_XTERM_COLOR_223      0xffd7af
#define VT_XTERM_COLOR_224      0xffd7d7
#define VT_XTERM_COLOR_225      0xffd7ff
#define VT_XTERM_COLOR_226      0xffff00
#define VT_XTERM_COLOR_227      0xffff5f
#define VT_XTERM_COLOR_228      0xffff87
#define VT_XTERM_COLOR_229      0xffffaf
#define VT_XTERM_COLOR_230      0xffffd7
#define VT_XTERM_COLOR_231      0xffffff
/* gray shades */
#define VT_XTERM_COLOR_232      0x000000
#define VT_XTERM_COLOR_233      0x121212
#define VT_XTERM_COLOR_234      0x1c1c1c
#define VT_XTERM_COLOR_235      0x262626
#define VT_XTERM_COLOR_236      0x303030
#define VT_XTERM_COLOR_237      0x3a3a3a
#define VT_XTERM_COLOR_238      0x444444
#define VT_XTERM_COLOR_239      0x4e4e4e
#define VT_XTERM_COLOR_240      0x585858
#define VT_XTERM_COLOR_241      0x626262
#define VT_XTERM_COLOR_242      0x6c6c6c
#define VT_XTERM_COLOR_243      0x767676
#define VT_XTERM_COLOR_244      0x808080
#define VT_XTERM_COLOR_245      0x8a8a8a
#define VT_XTERM_COLOR_246      0x949494
#define VT_XTERM_COLOR_247      0x9e9e9e
#define VT_XTERM_COLOR_248      0xa8a8a8
#define VT_XTERM_COLOR_249      0xb2b2b2
#define VT_XTERM_COLOR_250      0xbcbcbc
#define VT_XTERM_COLOR_251      0xc6c6c6
#define VT_XTERM_COLOR_252      0xd0d0d0
#define VT_XTERM_COLOR_253      0xdadada
#define VT_XTERM_COLOR_254      0xe4e4e4
#define VT_XTERM_COLOR_255      0xeeeeee

#define VT_XTERM_COLORMAP                                               \
    {                                                                   \
        VT_XTERM_COLOR_0,                                               \
        VT_XTERM_COLOR_1,                                               \
        VT_XTERM_COLOR_2,                                               \
        VT_XTERM_COLOR_3,                                               \
        VT_XTERM_COLOR_4,                                               \
        VT_XTERM_COLOR_5,                                               \
        VT_XTERM_COLOR_6,                                               \
        VT_XTERM_COLOR_7,                                               \
        VT_XTERM_COLOR_8,                                               \
        VT_XTERM_COLOR_9,                                               \
        VT_XTERM_COLOR_10,                                              \
        VT_XTERM_COLOR_11,                                              \
        VT_XTERM_COLOR_12,                                              \
        VT_XTERM_COLOR_13,                                              \
        VT_XTERM_COLOR_14,                                              \
        VT_XTERM_COLOR_15,                                              \
        VT_XTERM_COLOR_16,                                              \
        VT_XTERM_COLOR_17,                                              \
        VT_XTERM_COLOR_18,                                              \
        VT_XTERM_COLOR_19,                                              \
        VT_XTERM_COLOR_20,                                              \
        VT_XTERM_COLOR_21,                                              \
        VT_XTERM_COLOR_22,                                              \
        VT_XTERM_COLOR_23,                                              \
        VT_XTERM_COLOR_24,                                              \
        VT_XTERM_COLOR_25,                                              \
        VT_XTERM_COLOR_26,                                              \
        VT_XTERM_COLOR_27,                                              \
        VT_XTERM_COLOR_28,                                              \
        VT_XTERM_COLOR_29,                                              \
        VT_XTERM_COLOR_30,                                              \
        VT_XTERM_COLOR_31,                                              \
        VT_XTERM_COLOR_32,                                              \
        VT_XTERM_COLOR_33,                                              \
        VT_XTERM_COLOR_34,                                              \
        VT_XTERM_COLOR_35,                                              \
        VT_XTERM_COLOR_36,                                              \
        VT_XTERM_COLOR_37,                                              \
        VT_XTERM_COLOR_38,                                              \
        VT_XTERM_COLOR_39,                                              \
        VT_XTERM_COLOR_40,                                              \
        VT_XTERM_COLOR_41,                                              \
        VT_XTERM_COLOR_42,                                              \
        VT_XTERM_COLOR_43,                                              \
        VT_XTERM_COLOR_44,                                              \
        VT_XTERM_COLOR_45,                                              \
        VT_XTERM_COLOR_46,                                              \
        VT_XTERM_COLOR_47,                                              \
        VT_XTERM_COLOR_48,                                              \
        VT_XTERM_COLOR_49,                                              \
        VT_XTERM_COLOR_50,                                              \
        VT_XTERM_COLOR_51,                                              \
        VT_XTERM_COLOR_52,                                              \
        VT_XTERM_COLOR_53,                                              \
        VT_XTERM_COLOR_54,                                              \
        VT_XTERM_COLOR_55,                                              \
        VT_XTERM_COLOR_56,                                              \
        VT_XTERM_COLOR_57,                                              \
        VT_XTERM_COLOR_58,                                              \
        VT_XTERM_COLOR_59,                                              \
        VT_XTERM_COLOR_60,                                              \
        VT_XTERM_COLOR_61,                                              \
        VT_XTERM_COLOR_62,                                              \
        VT_XTERM_COLOR_63,                                              \
        VT_XTERM_COLOR_64,                                              \
        VT_XTERM_COLOR_65,                                              \
        VT_XTERM_COLOR_66,                                              \
        VT_XTERM_COLOR_67,                                              \
        VT_XTERM_COLOR_68,                                              \
        VT_XTERM_COLOR_69,                                              \
        VT_XTERM_COLOR_70,                                              \
        VT_XTERM_COLOR_71,                                              \
        VT_XTERM_COLOR_72,                                              \
        VT_XTERM_COLOR_73,                                              \
        VT_XTERM_COLOR_74,                                              \
        VT_XTERM_COLOR_75,                                              \
        VT_XTERM_COLOR_76,                                              \
        VT_XTERM_COLOR_77,                                              \
        VT_XTERM_COLOR_78,                                              \
        VT_XTERM_COLOR_79,                                              \
        VT_XTERM_COLOR_80,                                              \
        VT_XTERM_COLOR_81,                                              \
        VT_XTERM_COLOR_82,                                              \
        VT_XTERM_COLOR_83,                                              \
        VT_XTERM_COLOR_84,                                              \
        VT_XTERM_COLOR_85,                                              \
        VT_XTERM_COLOR_86,                                              \
        VT_XTERM_COLOR_87,                                              \
        VT_XTERM_COLOR_88,                                              \
        VT_XTERM_COLOR_89,                                              \
        VT_XTERM_COLOR_90,                                              \
        VT_XTERM_COLOR_91,                                              \
        VT_XTERM_COLOR_92,                                              \
        VT_XTERM_COLOR_93,                                              \
        VT_XTERM_COLOR_94,                                              \
        VT_XTERM_COLOR_95,                                              \
        VT_XTERM_COLOR_96,                                              \
        VT_XTERM_COLOR_97,                                              \
        VT_XTERM_COLOR_98,                                              \
        VT_XTERM_COLOR_99,                                              \
        VT_XTERM_COLOR_100,                                             \
        VT_XTERM_COLOR_101,                                             \
        VT_XTERM_COLOR_102,                                             \
        VT_XTERM_COLOR_103,                                             \
        VT_XTERM_COLOR_104,                                             \
        VT_XTERM_COLOR_105,                                             \
        VT_XTERM_COLOR_106,                                             \
        VT_XTERM_COLOR_107,                                             \
        VT_XTERM_COLOR_108,                                             \
        VT_XTERM_COLOR_109,                                             \
        VT_XTERM_COLOR_110,                                             \
        VT_XTERM_COLOR_111,                                             \
        VT_XTERM_COLOR_112,                                             \
        VT_XTERM_COLOR_113,                                             \
        VT_XTERM_COLOR_114,                                             \
        VT_XTERM_COLOR_115,                                             \
        VT_XTERM_COLOR_116,                                             \
        VT_XTERM_COLOR_117,                                             \
        VT_XTERM_COLOR_118,                                             \
        VT_XTERM_COLOR_119,                                             \
        VT_XTERM_COLOR_120,                                             \
        VT_XTERM_COLOR_121,                                             \
        VT_XTERM_COLOR_122,                                             \
        VT_XTERM_COLOR_123,                                             \
        VT_XTERM_COLOR_124,                                             \
        VT_XTERM_COLOR_125,                                             \
        VT_XTERM_COLOR_126,                                             \
        VT_XTERM_COLOR_127,                                             \
        VT_XTERM_COLOR_128,                                             \
        VT_XTERM_COLOR_129,                                             \
        VT_XTERM_COLOR_130,                                             \
        VT_XTERM_COLOR_131,                                             \
        VT_XTERM_COLOR_132,                                             \
        VT_XTERM_COLOR_133,                                             \
        VT_XTERM_COLOR_134,                                             \
        VT_XTERM_COLOR_135,                                             \
        VT_XTERM_COLOR_136,                                             \
        VT_XTERM_COLOR_137,                                             \
        VT_XTERM_COLOR_138,                                             \
        VT_XTERM_COLOR_139,                                             \
        VT_XTERM_COLOR_140,                                             \
        VT_XTERM_COLOR_141,                                             \
        VT_XTERM_COLOR_142,                                             \
        VT_XTERM_COLOR_143,                                             \
        VT_XTERM_COLOR_144,                                             \
        VT_XTERM_COLOR_145,                                             \
        VT_XTERM_COLOR_146,                                             \
        VT_XTERM_COLOR_147,                                             \
        VT_XTERM_COLOR_148,                                             \
        VT_XTERM_COLOR_149,                                             \
        VT_XTERM_COLOR_150,                                             \
        VT_XTERM_COLOR_151,                                             \
        VT_XTERM_COLOR_152,                                             \
        VT_XTERM_COLOR_153,                                             \
        VT_XTERM_COLOR_154,                                             \
        VT_XTERM_COLOR_155,                                             \
        VT_XTERM_COLOR_156,                                             \
        VT_XTERM_COLOR_157,                                             \
        VT_XTERM_COLOR_158,                                             \
        VT_XTERM_COLOR_159,                                             \
        VT_XTERM_COLOR_160,                                             \
        VT_XTERM_COLOR_161,                                             \
        VT_XTERM_COLOR_162,                                             \
        VT_XTERM_COLOR_163,                                             \
        VT_XTERM_COLOR_164,                                             \
        VT_XTERM_COLOR_165,                                             \
        VT_XTERM_COLOR_166,                                             \
        VT_XTERM_COLOR_167,                                             \
        VT_XTERM_COLOR_168,                                             \
        VT_XTERM_COLOR_169,                                             \
        VT_XTERM_COLOR_170,                                             \
        VT_XTERM_COLOR_171,                                             \
        VT_XTERM_COLOR_172,                                             \
        VT_XTERM_COLOR_173,                                             \
        VT_XTERM_COLOR_174,                                             \
        VT_XTERM_COLOR_175,                                             \
        VT_XTERM_COLOR_176,                                             \
        VT_XTERM_COLOR_177,                                             \
        VT_XTERM_COLOR_178,                                             \
        VT_XTERM_COLOR_179,                                             \
        VT_XTERM_COLOR_180,                                             \
        VT_XTERM_COLOR_181,                                             \
        VT_XTERM_COLOR_182,                                             \
        VT_XTERM_COLOR_183,                                             \
        VT_XTERM_COLOR_184,                                             \
        VT_XTERM_COLOR_185,                                             \
        VT_XTERM_COLOR_186,                                             \
        VT_XTERM_COLOR_187,                                             \
        VT_XTERM_COLOR_188,                                             \
        VT_XTERM_COLOR_189,                                             \
        VT_XTERM_COLOR_190,                                             \
        VT_XTERM_COLOR_191,                                             \
        VT_XTERM_COLOR_192,                                             \
        VT_XTERM_COLOR_193,                                             \
        VT_XTERM_COLOR_194,                                             \
        VT_XTERM_COLOR_195,                                             \
        VT_XTERM_COLOR_196,                                             \
        VT_XTERM_COLOR_197,                                             \
        VT_XTERM_COLOR_198,                                             \
        VT_XTERM_COLOR_199,                                             \
        VT_XTERM_COLOR_200,                                             \
        VT_XTERM_COLOR_201,                                             \
        VT_XTERM_COLOR_202,                                             \
        VT_XTERM_COLOR_203,                                             \
        VT_XTERM_COLOR_204,                                             \
        VT_XTERM_COLOR_205,                                             \
        VT_XTERM_COLOR_206,                                             \
        VT_XTERM_COLOR_207,                                             \
        VT_XTERM_COLOR_208,                                             \
        VT_XTERM_COLOR_209,                                             \
        VT_XTERM_COLOR_210,                                             \
        VT_XTERM_COLOR_211,                                             \
        VT_XTERM_COLOR_212,                                             \
        VT_XTERM_COLOR_213,                                             \
        VT_XTERM_COLOR_214,                                             \
        VT_XTERM_COLOR_215,                                             \
        VT_XTERM_COLOR_216,                                             \
        VT_XTERM_COLOR_217,                                             \
        VT_XTERM_COLOR_218,                                             \
        VT_XTERM_COLOR_219,                                             \
        VT_XTERM_COLOR_220,                                             \
        VT_XTERM_COLOR_221,                                             \
        VT_XTERM_COLOR_222,                                             \
        VT_XTERM_COLOR_223,                                             \
        VT_XTERM_COLOR_224,                                             \
        VT_XTERM_COLOR_225,                                             \
        VT_XTERM_COLOR_226,                                             \
        VT_XTERM_COLOR_227,                                             \
        VT_XTERM_COLOR_228,                                             \
        VT_XTERM_COLOR_229,                                             \
        VT_XTERM_COLOR_230,                                             \
        VT_XTERM_COLOR_231,                                             \
        VT_XTERM_COLOR_232,                                             \
        VT_XTERM_COLOR_233,                                             \
        VT_XTERM_COLOR_234,                                             \
        VT_XTERM_COLOR_235,                                             \
        VT_XTERM_COLOR_236,                                             \
        VT_XTERM_COLOR_237,                                             \
        VT_XTERM_COLOR_238,                                             \
        VT_XTERM_COLOR_239,                                             \
        VT_XTERM_COLOR_240,                                             \
        VT_XTERM_COLOR_241,                                             \
        VT_XTERM_COLOR_242,                                             \
        VT_XTERM_COLOR_243,                                             \
        VT_XTERM_COLOR_244,                                             \
        VT_XTERM_COLOR_245,                                             \
        VT_XTERM_COLOR_246,                                             \
        VT_XTERM_COLOR_247,                                             \
        VT_XTERM_COLOR_248,                                             \
        VT_XTERM_COLOR_249,                                             \
        VT_XTERM_COLOR_250,                                             \
        VT_XTERM_COLOR_251,                                             \
        VT_XTERM_COLOR_252,                                             \
        VT_XTERM_COLOR_253,                                             \
        VT_XTERM_COLOR_254,                                             \
        VT_XTERM_COLOR_255                                              \
    }

#endif /* __VT_COLOR_H__ */

