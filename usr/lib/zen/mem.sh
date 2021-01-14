#! /bin/sh

gcc -Wall -I. -I../../../usr/lib -I../../.. -O -o mem mem.c ../zen/hash.c -pthread

