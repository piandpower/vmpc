set(IPC_SA   app_wrapper/app_dialog.cpp app_wrapper/app_main.cpp ../../WDL/IPlug/IPlugStandalone.cpp)
set(IPC_AU   ../../WDL/IPlug/IPlugAU.cpp   ../../WDL/IPlug/dfx/dfx-au-utilities.c)
set(IPC_VST	 ../../WDL/IPlug/IPlugVST.cpp)
set(IPC_VST3 ../../WDL/IPlug/IPlugVST3.cpp)
set(IPC_RTAS ../../WDL/IPlug/IPlugRTAS.cpp)
set(IPC_AAX  ../../WDL/IPlug/IPlugAAX.cpp ../../WDL/IPlug/AAX/AAX_CIPlugParameters.cpp )

if(APPLE)
 find_library(AUDIO_UNIT AudioUnit)
 find_library(AUDIO_TOOLBOX AudioToolbox)
 if (NOT AUDIO_UNIT)
    message(FATAL_ERROR "AudioUnit not found")
 endif()
 if (NOT AUDIO_TOOLBOX)
    message(FATAL_ERROR "AudioToolbox not found")
 endif()
 
 find_library(CARBON Carbon)
 find_library(COCOA Cocoa)
 find_library(CORE_FND CoreFoundation)
 find_library(CORE_SVCS CoreServices)
 find_library(CORE_MIDI CoreMidi)
 find_library(CORE_AUDIO CoreAudio)
 find_library(APPKIT AppKit)

 set (OSX_FMWKS ${AUDIO_UNIT} ${AUDIO_TOOLBOX} ${CORE_MIDI} ${CORE_AUDIO} 
 	${COCOA} ${CARBON} ${CORE_FND} ${CORE_SVCS} ${APPKIT} )


endif(APPLE)

#-------------------------------------------------------------------------------------
# Create a Multi Target Example Project ...
#-------------------------------------------------------------------------------------
MACRO(create_example name)
	set (PLUGNAME ${ARGV0})
	set (PN_SRC ${PLUGNAME}_SRC )
	set (PN_HDR ${PLUGNAME}_HDR )
	set (PN_RSC ${PLUGNAME}_RSC )
	set (PN_IMG ${PLUGNAME}_IMG )

	if(APPLE)
		set (PLUGNAMESA   ${PLUGNAME})
	elseif(HAVE_ASIO)
		set (PLUGNAMESA   ${PLUGNAME})
	endif(APPLE)

	if(HAVE_AAX)
		set (PLUGNAMEAAX  ${PLUGNAME}AAX)
	endif(HAVE_AAX)
	
	if(HAVE_PT9)
		set (PLUGNAMEDPM  ${PLUGNAME}RTAS)
	endif(HAVE_PT9)
	
	if(HAVE_VST)
		set (PLUGNAMEVST  ${PLUGNAME}VST)
	endif(HAVE_VST)
	
	if(HAVE_VST3)
		set (PLUGNAMEVST3 ${PLUGNAME}VST3)
	endif(HAVE_VST3)
	
	if(HAVE_AU)
		set (PLUGNAMEAU   ${PLUGNAME}AU)
	endif(HAVE_AU)

	PROJECT ( ${PLUGNAME} )
	set(CMAKE_CXX_STANDARD 14)
	set(BINARY_NAME "${PLUGNAME}" )

	find_package(Boost 1.36.0)
	if(Boost_FOUND)
		include_directories(${Boost_INCLUDE_DIRS})
	endif()
	
	# Source code to build
	set(${PN_SRC} ${${PN_SRC}}   ${PLUGNAME}.cpp )
	
	
	file(GLOB _source_list ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp)
	set(${PN_SRC} ${${PN_SRC}}   ${_source_list} )
	
	if(WIN32)
	  set(${PN_SRC} ${${PN_SRC}} ${PLUGNAME}.rc)
	else(WIN32)
	  set(${PN_SRC} 
	  ${${PN_SRC}}  
	  ../../WDL/IPlug/IPlugCocoa.mm
	
	  #../../VST3_SDK/public.sdk/source/vst/vstbus.cpp
	  #../../VST3_SDK/public.sdk/source/vst/vstcomponentbase.cpp
	  #../../VST3_SDK/public.sdk/source/vst/vstsinglecomponenteffect.cpp
	  #../../VST3_SDK/public.sdk/source/vst/vstparameters.cpp
	  #../../VST3_SDK/base/source/fstring.cpp
	  #../../VST3_SDK/pluginterfaces/base/ustring.cpp
	  )
	  ADD_DEFINITIONS(
    	-D__MACOSX_CORE__ 
    	-DSWELL_FORCE_COCOA_TEXT 
    	-DCOCOA_PREFIX=v${PLUGNAME} 
    	-DSWELL_APP_PREFIX=Swell_v${PLUGNAME}
    	)
	endif(WIN32)

	# Source code headers
	set(${PN_HDR} ${${PN_HDR}} resource.h ${PLUGNAME}.h  )

	# Add Images resources for all OSX builds
	if(APPLE)
      set(GUI_TYPE MACOSX_BUNDLE)
      source_group(Resources\\img ".*png")
      set( ${PN_SRC} ${${PN_SRC}}  ${${PN_IMG}} )
      set_source_files_properties(${${PN_IMG}} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    else()
      SET(GUI_TYPE WIN32)
	endif()
	
	# Info.plist paths
	set(PLIST_PATHSA   ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-OSXAPP-Info.plist )
	set(PLIST_PATHVST  ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-VST2-Info.plist )
	set(PLIST_PATHVST3 ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-VST3-Info.plist )
	set(PLIST_PATHAU   ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-AU-Info.plist )
	set(PLIST_PATHRTAS ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-RTAS-Info.plist )
	set(PLIST_PATHAAX  ${CMAKE_CURRENT_SOURCE_DIR}/resources/${PLUGNAME}-AAX-Info.plist )

	#-------------------------------------------------------------------------------------
	# Add  Executable 
	#-------------------------------------------------------------------------------------
	if ( DEFINED PLUGNAMESA )
		if(APPLE)
		  set(APPICON_PATH resources/${PLUGNAME}.icns  )

		# Resources
		  set( ${PN_RSC}
			${CMAKE_CURRENT_SOURCE_DIR}/resources/English.lproj/MainMenu.xib 
			${CMAKE_CURRENT_SOURCE_DIR}/resources/English.lproj/InfoPlist.strings
			${APPICON_PATH}
			${PLIST_PATHSA}
			)
		
			SET_SOURCE_FILES_PROPERTIES( ${${PN_RSC}} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
		endif()

#	file(
#		GLOB_RECURSE _source_list 
#		LIST_DIRECTORIES false
#		RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
#		"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp*"
#		"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp*"
#	)
#	set(${PN_SRC} ${${PN_SRC}}   ${_source_list} )
		add_executable(${PLUGNAMESA} ${GUI_TYPE} ${${PN_SRC}} ${${PN_RSC}} ${APPICON_PATH} ${${PN_HDR}} ${IPC_SA}) 
		set_plug_props(${PLUGNAMESA} "SA_API=1")
	
		if(WIN32)
			set_target_properties(${PLUGNAMESA} PROPERTIES 
			 	LINK_FLAGS_DEBUG 	"/SUBSYSTEM:WINDOWS"
				LINK_FLAGS_RELEASE 	"/SUBSYSTEM:WINDOWS")
			target_compile_options(${PLUGNAMESA} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
			target_link_libraries(${PLUGNAMESA} asio)

		elseif(APPLE)
		  SET_TARGET_PROPERTIES(${PLUGNAMESA} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PLIST_PATHSA} )
		endif()
	endif()
  
	#-------------------------------------------------------------------------------------
	# Add  AudioUnit Plugin
	#-------------------------------------------------------------------------------------
	if(APPLE)
		if( DEFINED PLUGNAMEAU )
 			set (RCFILES "${CMAKE_CURRENT_SOURCE_DIR}/../../WDL/IPlug/IPlugAU.r" )

			add_library(${PLUGNAMEAU} MODULE 
				${${PN_SRC}} ${${PN_HDR}} ${PLIST_PATHAU}  ${IPC_AU} 
				"${CMAKE_CURRENT_SOURCE_DIR}/${PLUGNAME}.exp"
				 ${RCFILES} 
				)
				
	     	set_bundle_exp(${PLUGNAMEAU} "${PLIST_PATHAU}" "component" )
			set_plug_props(${PLUGNAMEAU} "AU_API=1")
			
    		foreach(ARCH ${CMAKE_OSX_ARCHITECTURES})
    	    	list(APPEND ARCHS -arch ${ARCH})
	    	endforeach()
 	    
	    	set (ORC_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PLUGNAMEAU}.rsrc" )
# 	   		get_directory_property(INC_DIRS INCLUDE_DIRECTORIES ) 
#     		message("Generating ${ORC_PATH} for ${INC_DIRS}" )

#  			add_custom_command( TARGET ${PLUGNAMEAU} POST_BUILD
#  			COMMAND /Developer/Tools/Rez  ARGS 
# 	   		  -d SystemSevenOrLater=1 -useDF -script Roman 
#     	      -useDF ${ARCHS}
#     	      -o ${ORC_PATH}
# 	   		  -i ${CMAKE_CURRENT_SOURCE_DIR}
# 	   		  -I ${CORE_AUDIO}/Versions/A/Headers/**
# 	   		  -I ${CORE_SVCS}/Versions/A/Headers/**
# 	   		  -i /Developer/SDKs/MacOSX${BUILD_SDK_VER}.sdk/usr/include
# 	   		  -isysroot /Developer/SDKs/MacOSX${BUILD_SDK_VER}.sdk
# 	   		  ${RCFILES} 
# 			)
		endif()
	endif()

	#-------------------------------------------------------------------------------------
	# Add  VST Plugin 
	#-------------------------------------------------------------------------------------
	if( DEFINED PLUGNAMEVST )
		add_library(${PLUGNAMEVST}  MODULE ${${PN_SRC}} ${${PN_HDR}} ${IPC_VST} )
        set_bundle(${PLUGNAMEVST} "${PLIST_PATHVST}" "vst" )
		set_plug_props(${PLUGNAMEVST} "VST_API=1 VST_FORCE_DEPRECATED")
		if (WIN32)
		target_compile_options(${PLUGNAMEVST} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
		endif (WIN32)
	endif()

	#-------------------------------------------------------------------------------------
	# Add  VST3 Plugin 
	#-------------------------------------------------------------------------------------
	if( DEFINED PLUGNAMEVST3 )

		set(VST3_ADDITIONAL_SRC "../../VST3_SDK/public.sdk/source/vst/vstbus.cpp"
	  "../../VST3_SDK/public.sdk/source/vst/vstcomponentbase.cpp"
	  "../../VST3_SDK/public.sdk/source/vst/vstsinglecomponenteffect.cpp"
	  "../../VST3_SDK/public.sdk/source/vst/vstparameters.cpp"
	  "../../VST3_SDK/base/source/fstring.cpp"
	  "../../VST3_SDK/pluginterfaces/base/ustring.cpp")

		set(FINAL_VST3_SRC ${${PN_SRC}}   ${VST3_ADDITIONAL_SRC} )
		add_library(${PLUGNAMEVST3} MODULE ${FINAL_VST3_SRC} ${${PN_HDR}} ${IPC_VST3} )
  		set_bundle(${PLUGNAMEVST3} "${PLIST_PATHVST3}" "vst3" )
		set_plug_props(${PLUGNAMEVST3} "VST3_API=1")
		target_link_libraries(${PLUGNAMEVST3} vst3)
		if(MSVC)
		set_target_properties(
		${PLUGNAMEVST3} PROPERTIES LINK_FLAGS "/DEF:IPlugVST3.def")
		target_compile_options(${PLUGNAMEVST3} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
		endif()
	endif()
	
	#-------------------------------------------------------------------------------------
	# Add  RTAS Plugin 
	#-------------------------------------------------------------------------------------
	if( DEFINED PLUGNAMEDPM )
		add_library(${PLUGNAMEDPM} MODULE ${${PN_SRC}} ${${PN_HDR}} ${IPC_RTAS} )
		set_bundle(${PLUGNAMEDPM} "${PLIST_PATHRTAS}" "dpm" )
		set_plug_props(${PLUGNAMEDPM} "RTAS_API=1")
		target_link_libraries(${PLUGNAMEDPM} dpm)
	endif()
	
	#-------------------------------------------------------------------------------------
	# Add  AAX Plugin 
	#-------------------------------------------------------------------------------------
	if( DEFINED PLUGNAMEAAX )
		add_library(${PLUGNAMEAAX} MODULE ${${PN_SRC}} ${${PN_HDR}} ${IPC_AAX} )
		set_bundle(${PLUGNAMEAAX} "${PLIST_PATHAAX}" "aax" )
		set_plug_props(${PLUGNAMEAAX} "AAX_API=1")
		target_link_libraries(${PLUGNAMEAAX} aax)
	endif()
	
	#-------------------------------------------------------------------------------------	
	
endMACRO()

#-------------------------------------------------------------------------------------
# Create a Multi Target Custom Project ...
#-------------------------------------------------------------------------------------
MACRO(create_iplug_project name)
	create_example(${ARGV0}) # just an alias of the examples build macros for now
endMACRO()
