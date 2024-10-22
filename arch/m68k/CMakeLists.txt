cmake_minimum_required(VERSION 3.13)

add_subdirectory("${TARGET_BOARD}")

target_sources(firmware PRIVATE init.S dump.c irq.S syscall.c exception.S)
target_include_directories(firmware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(firmware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
