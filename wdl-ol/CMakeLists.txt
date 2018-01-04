#-------------------------------------------------------------------------------------
# IPlug (wdl-fab) CMake Generation
#   Created by fabien on 2014/07/18
#   For bug reports please contact: fabien@onepost.net
#-------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8)

include (cmake/IPlugMacros.inc)
project(wdl-ol)
if(APPLE)

	# set the base sdk version:
	set(BUILD_SDK_VER 10.12)
	set(BUILD_SDK_MAXVER 101204)
	
	set(CMAKE_OSX_SYSROOT "macosx${BUILD_SDK_VER}")
	set(CMAKE_OSX_DEPLOYMENT_TARGET "${BUILD_SDK_VER}" )
 #	find_library( COREFOUNDATION_FRAMEWORK NAMES CoreFoundation)

endif(APPLE)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# simple, unified lib & bin dir for all libs and exe
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#-------------------------------------------------------------------------------------
# Find Packages
#-------------------------------------------------------------------------------------
# find_package(bzip2 required)
# find_package(png required)
# find_package(jpeg required)
# find_package(zlib required)
# find_package(OpenGL REQUIRED)
# find_package(GLU REQUIRED)
# find_package(SDL REQUIRED)
# Compile the resource file

if (APPLE)
  # find Rez tool for .r files (needed for rtas and audiounit builds)
  find_program(RC_COMPILER Rez PATHS /Developer/Tools)
  if(NOT RC_COMPILER)
    message(FATAL_ERROR "could not find Rez to build resources from .r file...")
  endif()
endif(APPLE)

#-------------------------------------------------------------------------------------
# Set Libraries Include & Lib Paths
#-------------------------------------------------------------------------------------

include_directories(
	# ${OPENAL_INCLUDES}
	# ${BZIP2_INCLUDE_DIR}
	# ${JPEG_INCLUDE_DIR}
	# ${PNG_INCLUDE_DIR}
	# ${ZLIB_INCLUDE_DIR}
	# ${OPENGL_INCLUDE_DIR}
	# ${GLU_INCLUDE_DIR}
	# ${SDL_INCLUDE_DIR}
#	${VORBIS_INCLUDE_DIR}
#	${OGG_INCLUDE_DIR}
#	${CMAKE_SOURCE_DIR}/Source
	${CMAKE_CURRENT_SOURCE_DIR}/WDL/IPlug	
	${CMAKE_CURRENT_SOURCE_DIR}/WDL	
	${CMAKE_CURRENT_SOURCE_DIR}/WDL/rtaudiomidi	
	${CMAKE_CURRENT_SOURCE_DIR}/../mpc/src
	${CMAKE_CURRENT_SOURCE_DIR}/../ctoot/src
	${CMAKE_CURRENT_SOURCE_DIR}/../moduru/src
	${CMAKE_CURRENT_SOURCE_DIR}/../freetype/include
	${CMAKE_CURRENT_SOURCE_DIR}/../rapidjson/include
	)

if(APPLE)
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/WDL/swell	)
endif(APPLE)

#-------------------------------------------------------------------------------------
# Set Common Definitions
#-------------------------------------------------------------------------------------
if (WIN32)
	add_definitions(-DOS_WIN)
	add_definitions(-D__WINDOWS_ASIO__)
	add_definitions(-D__WINDOWS_DS__)
	
else(WIN32)
	add_definitions(
		-DOS_OSX
		-D__MAC_OS_X_VERSION_MAX_ALLOWED=${BUILD_SDK_MAXVER}
		-DMAC_OS_X_VERSION_MAX_ALLOWED=${BUILD_SDK_MAXVER}
		)
#	find_package(Vorbis REQUIRED)
endif(WIN32)

if(MSVC)
  add_definitions(-DMSVC_RUNTIME=static)
  add_definitions(/arch:SSE2)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
  add_definitions(/wd4244 /wd4996 /wd4018 /wd4305)
  
elseif(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
  add_definitions(-D__MACOSX_CORE__ -DWDL_FFT_REALSIZE=8 
  	-DSWELL_FORCE_COCOA_TEXT -DSWELL_CLEANUP_ON_UNLOAD )
endif(MSVC)

configure_msvc_runtime()

set(variables
  CMAKE_C_FLAGS_DEBUG
  CMAKE_C_FLAGS_MINSIZEREL
  CMAKE_C_FLAGS_RELEASE
  CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG
  CMAKE_CXX_FLAGS_MINSIZEREL
  CMAKE_CXX_FLAGS_RELEASE
  CMAKE_CXX_FLAGS_RELWITHDEBINFO
)

dump_vars( "Initial build flags:" "${variables}" )

#Prefer static linking for plugins
set(BUILD_SHARED_LIBS OFF)  

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	  SET(IPLUG_LIBS ${IPLUG_LIBS} Winmm wsock32 Wininet)
   endif(MSVC)
endif(WIN32)

set(IPLUG_LIBS 
	# ${OPENAL_LIBRARY} 
	# ${PNG_LIBRARY} 
	# ${JPEG_LIBRARY} 
	# ${BZIP2_LIBRARIES} 
	# ${ZLIB_LIBRARIES} 
	# ${SDL_LIBRARY} 
	# ${GLU_LIBRARY} 
	# ${OPENGL_LIBRARY} 
#	${OGG_LIBRARY}
#	${VORBIS_LIBRARY} 
#	${DSP_LIBRARY} 
	)

#-------------------------------------------------------------------------------------
# Set Version number
#-------------------------------------------------------------------------------------
set (IPLUG_VERSION_MAJOR 0)
set (IPLUG_VERSION_MINOR 9)
set (IPLUG_VERSION_PATCH "0")
set(IPLUG_VERSION "${IPLUG_VERSION_MAJOR}.${IPLUG_VERSION_MINOR}")
set(IPLUG_VERSION_FULL "${IPLUG_VERSION}.${IPLUG_VERSION_PATCH}")

#-------------------------------------------------------------------------------------
# IPlug Third-Party SDKs Handling
#-------------------------------------------------------------------------------------

# Init 
set (HAVE_AXX 			OFF)
set (HAVE_PT9 			OFF)
set (HAVE_VST			OFF)
set (HAVE_VST3			OFF)
set (HAVE_ASIO			OFF)
set (HAVE_AU			OFF)
set (HAVE_CA			OFF) # external AU sdk in the CA_SDK folder for xcode4+ builds

# Auto-detection
message("Project sourcedir is ${PROJECT_SOURCE_DIR}")
if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/AAX_SDK/Interfaces)
	if(EXISTS "${PROJECT_SOURCE_DIR}/AAX_SDK/CMakeLists.txt")
		set (HAVE_AAX		ON)
	endif()
endif()

if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/PT9_SDK/AlturaPorts)
	if(EXISTS "${PROJECT_SOURCE_DIR}/PT9_SDK/CMakeLists.txt")
		set (HAVE_PT9		ON)
	endif()
endif()

if(WIN32)
  if(EXISTS ${PROJECT_SOURCE_DIR}/ASIO_SDK/asio.h)
	if(EXISTS "${PROJECT_SOURCE_DIR}/ASIO_SDK/CMakeLists.txt")
		set (HAVE_ASIO		ON)
	endif()
  endif()
endif(WIN32)

if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/VST3_SDK/public.sdk)
	if(EXISTS "${PROJECT_SOURCE_DIR}/VST3_SDK/CMakeLists.txt")
		set (HAVE_VST3		ON)
	endif()
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/VST_SDK/aeffect.h)
	if(EXISTS "${PROJECT_SOURCE_DIR}/VST_SDK/CMakeLists.txt")
		set (HAVE_VST		ON)
	endif()
endif()

if(APPLE)
	set (HAVE_AU			ON)
	if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/CA_SDK/AudioUnits)
		if(EXISTS "${PROJECT_SOURCE_DIR}/CA_SDK/CMakeLists.txt")
			set (HAVE_CA	ON)
		endif()
	endif()
endif(APPLE)

set(variables HAVE_AAX HAVE_PT9 HAVE_VST HAVE_VST3 HAVE_ASIO HAVE_AU HAVE_CA )
dump_vars( "SDKs used:" "${variables}" )

#-------------------------------------------------------------------------------------
# Adding Subdirectories to build on ...
#-------------------------------------------------------------------------------------

#	ASIO SDK
if(HAVE_ASIO)
	include_directories( "${PROJECT_SOURCE_DIR}/ASIO_SDK" )
	add_subdirectory(ASIO_SDK)
endif(HAVE_ASIO)

#	VST Development
if(HAVE_VST3)
	include_directories( "${PROJECT_SOURCE_DIR}/VST3_SDK" )
	add_subdirectory(VST3_SDK)
endif(HAVE_VST3)

if(HAVE_VST)
	include_directories( "${PROJECT_SOURCE_DIR}/VST_SDK" )
	add_subdirectory(VST_SDK)
endif(HAVE_VST)

if(HAVE_AAX)
	include_directories( 
		"${PROJECT_SOURCE_DIR}/AAX_SDK/Libs/AAXLibrary/include"
		"${PROJECT_SOURCE_DIR}/WDL/iPlug/AAX"
		"${PROJECT_SOURCE_DIR}/AAX_SDK/Interfaces"
		"${PROJECT_SOURCE_DIR}/AAX_SDK/Interfaces/ACF"
		"${PROJECT_SOURCE_DIR}/AAX_SDK/Extensions/GUI"
		)
	
	add_subdirectory(AAX_SDK)
endif(HAVE_AAX)

if(HAVE_PT9)
	include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/PT9_SDK" ) 
	add_subdirectory(PT9_SDK) # rtas sdk
endif()

# Apple Development
if(HAVE_CA)  # not tested yet ...
	include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/CA_SDK" )
	add_subdirectory(CA_SDK)
endif()

# get_directory_property(INC_DIRS INCLUDE_DIRECTORIES ) 
# message("INCLUDE DIRS: ${INC_DIRS}" )

# Common
add_subdirectory(WDL) # wdl lice iplug libs
add_subdirectory(IPlugExamples)

# Your Custom Projects below that root folder:
if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/IPlugProjects)
  add_subdirectory(IPlugProjects)
endif()

