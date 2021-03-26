#! /bin/sh

#qemu-system-i386 -s -S -soundhw ac97,sb16 -cdrom zero.iso -m 512 -d guest_errors
qemu-system-i386 -s -S -vga std -cdrom zero.iso -m 2048

