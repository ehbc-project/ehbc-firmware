#!/bin/sh

dd if=/dev/zero of=disk.img bs=1024K count=16

mformat -F -i disk.img
mmd -i disk.img ::/EFW
mmd -i disk.img ::/EFW/BOOT
mcopy -i disk.img build/examples/hello_world/HELOWRLD.EFW ::/EFW/BOOT/HELOWRLD.EFW
