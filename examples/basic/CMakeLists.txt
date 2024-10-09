cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

# Target Configurations
add_executable(example_basic basic.c mpool.c)
set_target_properties(example_basic PROPERTIES SUFFIX ".elf")

# Compile Options
target_compile_features(example_basic PUBLIC c_std_11)
target_compile_features(example_basic PUBLIC cxx_std_14)
target_compile_options(example_basic PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)
target_link_libraries(example_basic gcc)
target_link_libraries(example_basic libehbcfw)
target_link_libraries(example_basic crtbegin)
target_link_libraries(example_basic crtend)

target_include_directories(example_basic PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(example_basic PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(example_basic PUBLIC libehbcfw::libehbcfw)
target_link_options(example_basic PUBLIC -T "${CMAKE_SOURCE_DIR}/libehbcfw/arch/m68k/proto1/ldscript.x")

add_custom_target(example_basic_bin ALL
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:example_basic>" example_basic.bin
    DEPENDS example_basic
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Creating binary file")
