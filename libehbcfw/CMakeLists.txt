cmake_minimum_required(VERSION 3.13)

# Target Configurations
add_library(libehbcfw STATIC)
set_target_properties(libehbcfw PROPERTIES OUTPUT_NAME "ehbcfw")
add_library(crtbegin OBJECT)
add_library(crtend OBJECT)

# Compile Options
target_compile_features(libehbcfw PUBLIC c_std_11)
target_compile_features(libehbcfw PUBLIC cxx_std_14)
target_compile_options(libehbcfw PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)
target_link_libraries(libehbcfw gcc)

target_include_directories(libehbcfw INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(libehbcfw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(libehbcfw PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(libehbcfw PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

# Subdirectories
add_subdirectory(arch)
add_subdirectory(src)
