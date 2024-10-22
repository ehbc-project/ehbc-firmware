cmake_minimum_required(VERSION 3.13)

add_subdirectory("${TARGET_BOARD}")

target_sources(libehbcfw PRIVATE setjmp.S)
target_include_directories(libehbcfw PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(libehbcfw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(libehbcfw INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
