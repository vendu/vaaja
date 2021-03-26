#! /bin/sh

qemu-system-i386 -cdrom zero.iso -m 1024
#qemu-system-i386 -soundhw ac97,sb16 -cdrom zero.iso -m 2048
#qemu-system-i386 -cdrom zero.iso -m 512 -d guest_errors

