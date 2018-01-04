#-------------------------------------------------------------------------------------
# Useful, reusable macros definitions for iPlug cmake
#   Created by fabien on 2014/07/18
#   For bug reports please contact: fabien@onepost.net
#-------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------
macro(configure_msvc_runtime)
  if(MSVC)
    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    
    # Set compiler options.
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
    
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
    
  else(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
    
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse3 -mssse3")
    set (CMAKE_CXX_FLAGS 
    	"${CMAKE_CXX_FLAGS}  -Wreturn-type -Wunused-variable -Wno-deprecated-declarations -Wno-write-strings -Wno-reorder -msse3 -mssse3" )

  endif()
endmacro()

#-------------------------------------------------------------------------------------
macro(get_subdir_list result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

#-------------------------------------------------------------------------------------
macro(dump_vars title list_vars)
	message(STATUS "${ARGV0}")
	foreach(variable ${ARGV1})
	  message(STATUS "  '${variable}': ${${variable}}")
	endforeach()
	message(STATUS "")
endmacro()

#-------------------------------------------------------------------------------------
MACRO(set_bundle a_project a_pname an_ext)
  
  set(PLUGEXT "${ARGV2}" )

  if(APPLE)
    if (${ARGV2} MATCHES "(aax)")
    	set(PLUGEXT "${ARGV2}plugin" )
    endif()

    if (${ARGV2} MATCHES "(aax|dpm)")
    	set(PKGINF "${PROJECT_SOURCE_DIR}/../../WDL/IPlug/AAX/PkgInfo" )
    else()
    	set(PKGINF "${PROJECT_SOURCE_DIR}/../../WDL/IPlug/PkgInfo" )
    endif()

	set_target_properties( ${ARGV0} PROPERTIES
    	BUNDLE 1
		BUNDLE_EXTENSION  ${PLUGEXT}
    	XCODE_ATTRIBUTE_WRAPPER_EXTENSION ${PLUGEXT}  #sets the extension to .${PLUGEXT}
    	XCODE_ATTRIBUTE_MACH_O_TYPE mh_bundle
#    	XCODE_ATTRIBUTE_INFOPLIST_FILE ${ARGV1}
    	MACOSX_BUNDLE_INFO_PLIST ${ARGV1}
    )
    set_source_files_properties( ${ARGV1} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    
    
    add_custom_command(
    	TARGET ${ARGV0}
    	POST_BUILD
    	COMMAND cp
    	ARGS "${PKGINF}" "$<TARGET_FILE_DIR:${ARGV0}>/.."
  	)
#	message(STATUS "Post Cmd: cp ${PKGINF} $<TARGET_FILE_DIR:${ARGV0}>/..")
	message(STATUS "bundle plist is ${ARGV1}")
  else(APPLE)
	message(STATUS "set_bundle(${ARGV0} ${ARGV1} ${ARGV2})")
	if (WIN32)
		# handle extension dll settings
		if( ${ARGV2} STREQUAL "vst3" )
			set(addext 1)
		elseif( ${ARGV2} STREQUAL "dpm" )
			set(addext 1)
		else()
		# vst aax dont need a change here
			set(addext 0)
		endif()
		if( ${addext} STREQUAL "1" )
			set_target_properties(${ARGV0} PROPERTIES SUFFIX ".${PLUGEXT}") 
		endif()
	endif()
  endif(APPLE)
endMACRO()

#-------------------------------------------------------------------------------------
MACRO(set_bundle_exp a_project a_pname an_ext)
	set_bundle(${ARGV0} ${ARGV1} ${ARGV2} )
	set_target_properties( ${ARGV0} PROPERTIES
	    LINK_FLAGS "-Wl,-exported_symbols_list,\"${CMAKE_CURRENT_SOURCE_DIR}/${PLUGNAME}.exp\"")
endMACRO()

#-------------------------------------------------------------------------------------
MACRO(set_plug_props a_project compile_defs)
	set_target_properties( ${ARGV0} PROPERTIES
        COMPILE_DEFINITIONS "${ARGV1}"
		)
		
	if(APPLE)
		set_target_properties( ${ARGV0} PROPERTIES
			OUTPUT_NAME ${PLUGNAME}
			XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME "NO"
			)
	elseif(WIN32)
		if(${ARGV0} MATCHES "VST3" ) # ext is already not dll so keep the plug name short
			set_target_properties( ${ARGV0} PROPERTIES	OUTPUT_NAME ${PLUGNAME} )
		endif()
	endif(APPLE)
	target_link_libraries(${ARGV0} wdl lice iplug jnet eel plush png zlib mpc moduru ctoot rtam freetype ${IPLUG_LIBS} ${OSX_FMWKS} )
	if(WIN32)
		target_link_libraries(${ARGV0} comctl32 dsound)
	endif(WIN32)

endMACRO()
