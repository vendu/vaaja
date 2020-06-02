#! /bin/sh

gcc -Wall -g -DTEST_FXP64 -O -I.. -o fxp64 fxp64.c -lm
