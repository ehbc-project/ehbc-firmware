cmake_minimum_required(VERSION 3.13)

add_library(resources STATIC VGA8.F16.obj)
set_target_properties(resources PROPERTIES LINKER_LANGUAGE C)

add_custom_command(
    OUTPUT VGA8.F16.obj
    COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}" && ${CMAKE_OBJCOPY} -I binary -O elf32-m68k --rename-section .data=.rodata,alloc,load,readonly,data,contents VGA8.F16 "${CMAKE_CURRENT_BINARY_DIR}/VGA8.F16.obj"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/VGA8.F16"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Converting resource object VGA8.F16.obj")
set_source_files_properties(VGA8.F16.obj PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true)

target_link_libraries(firmware resources)
