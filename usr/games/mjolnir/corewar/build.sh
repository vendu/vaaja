#! /bin/sh

gcc -DCW_DEBUG -DCW_BIG_CORE -DTEST_CW -g -Wall -Wextra -Wundef -I.. -I../../../lib -O -o mars cw.c rc.c ../../../lib/prng/randmt32.c
gcc -DZEUS -DZEUSX11 -DZEUSIMLIB2 -DCW_DEBUG -DCW_BIG_CORE -DTEST_CW -g -Wall -Wextra -Wundef -I.. -I../../../lib -O -o zeus cw.c rc.c ../../../lib/prng/randmt32.c zeus.c x11.c -lX11 -lImlib2 -lSDL2 -lSDL2_image

