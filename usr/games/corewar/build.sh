#! /bin/sh

CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wextra -Wpointer-arith -Wuninitialized -Winit-self -Wmissing-include-dirs -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wtrampolines -Wfloat-equal -Wshadow -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wbad-function-cast -Wc99-c11-compat -Wcast-align -Wwrite-strings -Wdate-time -Wjump-misses-init -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wnormalized -Wopenmp-simd -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wvector-operation-performance -Wvla -Wstack-protector -Woverlength-strings -Wno-designated-init -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"

gcc $CCWARNFLAGS -I../../lib -I.. -O -o cw cw.c rc.c -pthread
gcc -g $CCWARNFLAGS -DZEUS=1 -DZEUSWINX11 -DZEUSIMLIB2 -I../../lib -I.. -O -g -o zeus cw.c rc.c zeus.c zeus-x11.c -lX11 -lImlib2 -pthread
#gcc $CCWARNFLAGS -DZEUS=1 -DZEUSWINSDL -I../../lib -I.. -O -g -o zeus-sdl cw.c rc.c zeus.c zeus-sdl.c -lSDL2 -lSDL2_image

