cmake_minimum_required(VERSION 3.13)

target_sources(firmware PRIVATE start.asm io.asm cfgutil.c init.c exception.c scu.c debug.c)
target_link_options(firmware PUBLIC -T "${CMAKE_CURRENT_SOURCE_DIR}/ldscript.x")
set_target_properties(firmware PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ldscript.x")
