#! /bin/sh

gcc -nostdinc -O -I. -I.. -I../.. -I../../usr/lib -I../../usr/lib/c -o taika *.c

