cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0076 NEW)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CheckLanguage)
include(CheckTypeSize)
include(CMakePrintHelpers)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/M68kUnknownElfToolchain.cmake")

project("ehbc-firmware")

# Build Configurations
set(TARGET "m68k-proto1" CACHE STRING "Target name to build")
string(REPLACE "-" ";" TARGET ${TARGET})
list(GET TARGET 0 TARGET_ARCH)
list(GET TARGET 1 TARGET_BOARD)
enable_language(C ASM_VASM)

# Target Configurations
add_executable(firmware)
set_target_properties(firmware PROPERTIES SUFFIX ".elf")

# Compiler Tests
check_type_size("void*" VOIDPTR_SIZE)
if (${VOIDPTR_SIZE} GREATER 4)
    set(MADDR_MSIZE_64BIT 1)
endif()

configure_file("config.h.in" "config.h")

# Dependencies
add_subdirectory(libehbcfw)
add_subdirectory(utils)

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
target_link_libraries(firmware gcc)
target_link_libraries(firmware libehbcfw)

target_include_directories(firmware PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(firmware PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_include_directories(firmware PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(firmware PUBLIC libehbcfw::libehbcfw)

# Subdirectories
add_subdirectory(arch)
add_subdirectory(src)
add_subdirectory(rsrc)
add_subdirectory(examples)

# Additional Targets
add_custom_target(romfs_image ALL
    COMMAND "${CMAKE_BINARY_DIR}/utils/romfs/mkromfs"
    DEPENDS util_romfs
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating ROM filesystem image")
add_dependencies(firmware romfs_image)

add_custom_target(rom_image ALL
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:firmware>" firmware.bin
    DEPENDS firmware
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating binary file")

add_custom_target(elfinfo ALL
    COMMAND ${CMAKE_OBJDUMP} -xdS "$<TARGET_FILE:firmware>" > firmware.objdump
    DEPENDS firmware
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating ELF analysis"
    USES_TERMINAL)
