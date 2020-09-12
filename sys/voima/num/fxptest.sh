#! /bin/sh

gcc -g -DTEST_FXP -I../.. -I../../../usr/lib -O -o fxptest fxp/fxp.c -lm

