#! /bin/sh

qemu-system-i386 -vga std -soundhw sb16 -cdrom zero.iso -m 2048 -smp 2

