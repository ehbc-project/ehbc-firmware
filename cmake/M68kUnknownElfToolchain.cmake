set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR m68k)

set(CMAKE_SYSROOT)

find_program(CMAKE_C_COMPILER
    "m68k-unknown-elf-gcc"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH
    REQUIRED)
set(CMAKE_C_COMPILER_TARGET     m68k-unknown-elf)
set(CMAKE_C_FLAGS               "${CMAKE_C_FLAGS} -ffreestanding -nostdlib -march=68030 -m68881")

find_program(CMAKE_CXX_COMPILER
    "m68k-unknown-elf-g++"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH
    REQUIRED)
set(CMAKE_CXX_COMPILER_TARGET   m68k-unknown-elf)
set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -ffreestanding -nostdlib -march=68030 -m68881")

find_program(CMAKE_ASM_VASM_COMPILER
    "vasmm68k_mot"
    HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH
    REQUIRED)
set(CMAKE_ASM_VASM_COMPILER_TARGET   m68k-unknown-elf)
set(CMAKE_ASM_VASM_FLAGS             "${CMAKE_ASM_VASM_FLAGS} -m68030 -m68882")

set(_BINUTILS_LIST AR;NM;OBJCOPY;OBJDUMP;RANLIB;READELF;STRIP)

foreach(TOOL_NAME IN LISTS _BINUTILS_LIST)
    string(TOLOWER ${TOOL_NAME} _TOOL_BIN_NAME)
    find_program(CMAKE_${TOOL_NAME}
        "m68k-unknown-elf-${_TOOL_BIN_NAME}"
        HINTS "/usr" "/usr/local" "/opt/homebrew" ENV PATH
        REQUIRED)
    unset(_TOOL_BIN_NAME)
endforeach()

unset(_BINUTILS_LIST)

