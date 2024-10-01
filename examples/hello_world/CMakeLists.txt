cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

# Target Configurations
add_executable(example_hello_world main.c)
set_target_properties(example_hello_world PROPERTIES OUTPUT_NAME "HELOWRLD")
set_target_properties(example_hello_world PROPERTIES SUFFIX ".EFW")

# Compile Options
target_compile_features(example_hello_world PUBLIC c_std_11)
target_compile_features(example_hello_world PUBLIC cxx_std_14)
target_compile_options(example_hello_world PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)
target_link_libraries(example_hello_world gcc)
target_link_libraries(example_hello_world libehbcfw)
target_link_libraries(example_hello_world crtbegin)
target_link_libraries(example_hello_world crtend)

target_include_directories(example_hello_world PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(example_hello_world PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(example_hello_world PUBLIC libehbcfw)

