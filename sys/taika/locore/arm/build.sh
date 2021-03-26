#! /bin/sh

arm-none-eabi-gcc -ffreestanding -T kern.lds -Wall -I../../../usr/lib -I../.. -I../../.. -O -o test setup.S main.c trap.c vm.c
