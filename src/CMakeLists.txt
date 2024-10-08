cmake_minimum_required(VERSION 3.13)

target_sources(firmware PRIVATE device.c debug.c main.c monitor.c memmap.c ringbuf.c spinlock.c)

add_subdirectory(basic)
add_subdirectory(crypto)
add_subdirectory(fs)
add_subdirectory(hw)
