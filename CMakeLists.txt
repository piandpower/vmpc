cmake_minimum_required(VERSION 2.8)

option(BUILD_DYNAMIC "Build dynamic (i.e. for Unreal)"
       OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_SUPPRESS_REGENERATION true)

add_subdirectory(mpc)
add_subdirectory(ctoot)
add_subdirectory(moduru)
add_subdirectory(wdl-ol)
add_subdirectory(freetype)

project(vmpc)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "VMPCWDL" )