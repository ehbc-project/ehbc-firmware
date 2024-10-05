cmake_minimum_required(VERSION 3.13)

add_subdirectory("${TARGET_BOARD}")

target_sources(libehbcfw PRIVATE syscall_aio.c)
target_include_directories(libehbcfw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
