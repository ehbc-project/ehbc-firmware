cmake_minimum_required(VERSION 3.13)

target_sources(firmware PRIVATE ata.c vga.c vgaregs.c ps2kbms.c rtc.c mc68681.c floppy.c)
