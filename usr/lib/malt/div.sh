#! /bin/sh

gcc -Wall -g -DTEST_DIV -O -I.. -o div div.c irp.c util.c -lm

