cmake_minimum_required(VERSION 3.13)

target_sources(libehbcfw PRIVATE bswap.c disk.c elf.c mm.c mpool.c syscall_aio.c syscall_video.c syscall_storage.c syscall_rtc.c)

add_subdirectory(stdc)
