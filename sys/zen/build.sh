#! /bin/sh

# generate nice-to-priority lookup table
cd gen
./gennice.sh
cd ..
# compile the system
gcc -D__v0__ -DPTHREAD -g -O -Wall -Wextra -I.. -I../../usr/lib -o zen *.c sched/*.c net/*.c ../../usr/lib/zen/fastudiv16.c -pthread

