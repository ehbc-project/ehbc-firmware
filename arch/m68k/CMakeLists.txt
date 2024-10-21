cmake_minimum_required(VERSION 3.13)

add_subdirectory("${TARGET_BOARD}")

target_sources(firmware PRIVATE init.asm dump.c irq.asm syscall.c exception.asm)
target_include_directories(firmware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(firmware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
