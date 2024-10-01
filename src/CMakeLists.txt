cmake_minimum_required(VERSION 3.13)

target_sources(firmware PRIVATE main.c monitor.c hw/vga.c hw/vgaregs.c hw/ps2kbms.c hw/rtc.c hw/mc68681.c hw/floppy.c)
