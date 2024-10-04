cmake_minimum_required(VERSION 3.13)

project("romfs-utils")

# Target Configurations
add_executable(mkromfs mkromfs.c romfs.c)
add_executable(dumpromfs dumpromfs.c romfs.c)

# Compile Options (mkromfs)
target_compile_features(mkromfs PUBLIC c_std_11)
target_compile_features(mkromfs PUBLIC cxx_std_14)
target_compile_options(mkromfs PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)

# Compile Options (dumpromfs)
target_compile_features(dumpromfs PUBLIC c_std_11)
target_compile_features(dumpromfs PUBLIC cxx_std_14)
target_compile_options(dumpromfs PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)
