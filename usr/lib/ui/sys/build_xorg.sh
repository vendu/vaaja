#! /bin/sh

gcc -I../.. -DUITESTXORG=1 -o testxorg xorg.c -lX11

