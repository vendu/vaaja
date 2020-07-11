#! /bin/sh

gcc -DV0_PAPILIO_PRO -DTEST_VM -g -O -Wall -I.. -I../../../sys -o vm *.c ../mt/mtx.c

