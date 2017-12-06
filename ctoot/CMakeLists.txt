cmake_minimum_required(VERSION 2.8)
include (../wdl-ol/cmake/IPlugMacros.inc)
project(ctoot)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

include_directories( ${_src_root_path} )
include_directories( ${PARENT_DIR}/moduru/src )
include_directories( ${PARENT_DIR}/mpc/src )
#include_directories( ${PARENT_DIR}/rtaudio )
#include_directories( ${PARENT_DIR}/rtmidi )

find_package(Boost 1.36.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
  
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${_src_root_path}/*.cpp*"
    "${_src_root_path}/*.hpp*"
)

add_library(ctoot ${_source_list})

if(MSVC)
  target_compile_options(mpc PRIVATE "/MD$<$<CONFIG:Debug>:d> /MP")
	#configure_msvc_runtime()
endif(MSVC)

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()