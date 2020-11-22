#! /bin/sh

gcc -DCW_DEBUG -DCW_BIG_CORE -DTEST_CW -g -Wall -Wextra -Wundef -I.. -I../../lib -O -o cw cw.c rc.c sdl.c ../../lib/prng/randmt32.c -lSDL2 -lSDL2_image

