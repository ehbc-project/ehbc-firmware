cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)

# find mtools
find_program(DD
    "dd"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH)
find_program(MFORMAT
    "mformat"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH)
find_program(MCOPY
    "mcopy"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH)

# Target Configurations
add_executable(example_basic my_basic.c shell.c)
set_target_properties(example_basic PROPERTIES SUFFIX ".elf")

# Compile Options
target_compile_features(example_basic PUBLIC c_std_11)
target_compile_features(example_basic PUBLIC cxx_std_14)
target_compile_options(example_basic PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
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

add_custom_target(example_basic_bin
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:example_basic>" BOOT.BIN
    DEPENDS example_basic
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Creating binary file")

if (DD STREQUAL "DD-NOTFOUND" OR MFORMAT STREQUAL "MFORMAT-NOTFOUND" OR MCOPY STREQUAL "MCOPY-NOTFOUND")
    message(NOTICE "The dd, mformat, and mcopy commands required to create a disk image are missing. Disk image target is not generated.")
else()
    add_custom_target(example_image_basic
        COMMAND ${DD} if=/dev/zero of=floppy.img bs=1024 count=1440
        COMMAND ${MFORMAT} -i floppy.img
        COMMAND ${MCOPY} -i floppy.img BOOT.BIN ::/
        DEPENDS example_basic_bin
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Creating floppy disk image")
endif()
