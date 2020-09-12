#! /bin/sh

gcc -mfpmath=387 -O -Wall -o x87test x87.c -lm

