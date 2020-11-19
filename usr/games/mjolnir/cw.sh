#! /bin/sh

gcc -DCW_DEBUG -DTEST_CW -g -Wall -Wextra -Wundef -I.. -I../../lib -O -o cw cw.c rc.c sdl.c ../../lib/prng/randmt32.c -lSDL2 -lSDL2_image

