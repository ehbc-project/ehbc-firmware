cmake_minimum_required(VERSION 3.13)

add_subdirectory("${TARGET_BOARD}")

target_sources(firmware PRIVATE init.asm dump.c)
target_include_directories(firmware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
