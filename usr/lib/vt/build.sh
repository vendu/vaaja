#! /bin/bash

gcc -D__v0__ -Wextra -Wundef -Wpointer-arith -DTEST_VT -I.. -g -Wall -O -o test *.c ../ui/*.c ../ui/sys/zero.c ../ui/sys/xorg.c ../zero/unix.c -lX11

