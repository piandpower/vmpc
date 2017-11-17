cmake_minimum_required(VERSION 2.8)
include (../wdl-ol/cmake/IPlugMacros.inc)
project(mpc)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

include_directories( ${_src_root_path} )
include_directories( ${PARENT_DIR}/moduru/src )
include_directories( ${PARENT_DIR}/ctoot/src )
include_directories( ${PARENT_DIR}/rapidjson/include )

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

if(MSVC)
  #add_definitions(-DMSVC_RUNTIME=static)
  add_compile_options(/MP)
  #add_compile_options(/MD)
configure_msvc_runtime()
endif(MSVC)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
  set(CMAKE_CXX_STANDARD 14)
endif(APPLE)

add_library(mpc ${_source_list})

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()