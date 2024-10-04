cmake_minimum_required(VERSION 3.13)

include(ExternalProject)

ExternalProject_Add(util_romfs
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/romfs"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/romfs"
    INSTALL_COMMAND "")
