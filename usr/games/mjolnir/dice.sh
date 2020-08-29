#! /bin/sh

gcc -g -Wall -fno-builtin -DDICE_SRAND_TIME=0 -DTEST=1 -I.. -I../../lib -o dice dice.c ../../lib/prng/randmt32.c

