cmake_minimum_required(VERSION 3.13)

target_sources(firmware PRIVATE cfgutil.c device.c debug.c exec.c main.c monitor.c memmap.c ringbuf.c spinlock.c splash.c)

add_subdirectory(crypto)
add_subdirectory(fs)
add_subdirectory(hw)
