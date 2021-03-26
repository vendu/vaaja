#! /bin/sh

qemu-system-i386 -s -S --no-reboot --no-shutdown -vga std -soundhw ac97 -cdrom zero.iso -m 2048 -smp 2

