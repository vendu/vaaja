#! /bin/sh

gcc -O0 -Wall -I. -I../../lib -o coptest cop/cop.c
gcc -O0 -Wall -I. -I../../lib -o foptest fop/fop.c
