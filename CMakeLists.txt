cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CheckLanguage)
include(CheckTypeSize)

find_file(CMAKE_TOOLCHAIN_FILE
    M68kUnknownElfToolchain.cmake
    HINTS ENV PATH
    PATH_SUFFIXES ../share/cmake/toolchains
    REQUIRED)

project("ehbc-firmware")

# Build Configurations
set(TARGET "m68k-proto1" CACHE STRING "Target name to build")
string(REPLACE "-" ";" TARGET ${TARGET})
list(GET TARGET 0 TARGET_ARCH)
list(GET TARGET 1 TARGET_BOARD)

# Target Configurations
add_executable(firmware)
set_target_properties(firmware PROPERTIES SUFFIX ".elf")
add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:firmware>" firmware.bin
    BYPRODUCTS firmware.bin
    COMMENT "Creating binary file")

add_custom_target(elfinfo ALL
    COMMAND ${CMAKE_OBJDUMP} -sSxr "$<TARGET_FILE:firmware>" > firmware.objdump
    DEPENDS firmware
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating ELF analysis"
    USES_TERMINAL)

# Compiler Tests
check_language(ASM)
if(${CMAKE_ASM_COMPILER_ID} NOT EQUAL "GNU")
    message(FATAL_ERROR "GNU Assembler required.")
else()
    enable_language(ASM)
endif()
if (NOT DEFINED CMAKE_C_BYTE_ORDER)
    include(TestBigEndian)
    test_big_endian(BYTE_ORDER_BIG_ENDIAN)
    if (NOT ${BYTE_ORDER_BIG_ENDIAN})
        unset(BYTE_ORDER_BIG_ENDIAN)
    endif()
else()
    if (${CMAKE_C_BYTE_ORDER} STREQUAL BIG_ENDIAN)
        set(BYTE_ORDER_BIG_ENDIAN 1)
    endif()
endif()
check_type_size("void*" VOIDPTR_SIZE)
if (${VOIDPTR_SIZE} GREATER 4)
    set(MADDR_MSIZE_64BIT 1)
endif()

# Subdirectories
add_subdirectory(arch)
add_subdirectory(drivers)
add_subdirectory(core)

# Compile Options
target_compile_features(firmware PUBLIC c_std_11)
target_compile_features(firmware PUBLIC cxx_std_14)
target_compile_options(firmware PUBLIC 
    $<$<COMPILE_LANGUAGE:C,CXX>:
        -Werror
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -pedantic
        -pedantic-errors>)

target_include_directories(firmware PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(firmware PRIVATE "${CMAKE_SOURCE_DIR}")
target_include_directories(firmware PRIVATE "${CMAKE_BINARY_DIR}")

configure_file("config.h.in" "config.h")
