cmake_minimum_required(VERSION 2.8)
include (../wdl-ol/cmake/IPlugMacros.inc)

project(moduru)

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

find_package(Boost 1.36.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()
  
include_directories( ${_src_root_path} )

file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${_src_root_path}/*.c*"
    "${_src_root_path}/*.cpp*"
    "${_src_root_path}/*.hpp*"
)

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
  set(CMAKE_CXX_STANDARD 14)
endif(APPLE)


if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

if (LINUX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_library(moduru ${_source_list})

if(MSVC)
	#set_target_properties(moduru PROPERTIES COMPILE_FLAGS_RELEASE "/MD /MP")
	#set_target_properties(moduru PROPERTIES COMPILE_FLAGS_DEBUG "/MDd /MP")
    target_compile_options(mpc PRIVATE "/MD$<$<CONFIG:Debug>:d> /MP")
	#configure_msvc_runtime()
endif(MSVC)

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()
