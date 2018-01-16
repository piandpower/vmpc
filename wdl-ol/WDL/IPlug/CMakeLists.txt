add_subdirectory(doc)

SET(IPLUG_SRC
	Hosts.cpp
	IBitmapMonoText.cpp
	IControl.cpp
	IGraphics.cpp
	IParam.cpp
	IPlugBase.cpp
	IPlugStructs.cpp
	IPopupMenu.cpp
	Log.cpp
	IControlGroup.cpp
	IPlugGUIResize.cpp
	
#	app_main.cpp
#	app_dialog.cpp

)

SET(IPLUG_HEADERS
	Containers.h
	Hosts.h
	IAutoGUI.h
	IBitmapMonoText.h
	IControl.h
	IGraphics.h
	IGraphicsCarbon.h
	IGraphicsCocoa.h
	IGraphicsMac.h
	IGraphicsWin.h
	IKeyboardControl.h
	IMidiQueue.h
	IParam.h
	IPlugAAX.h
	IPlugAU.h
	IPlugBase.h
	IPlugOSDetect.h
	IPlugRTAS.h
	IPlugStandalone.h
	IPlugStructs.h
	IPlugVST.h
	IPlugVST3.h
	IPlug_include_in_plug_hdr.h
	IPlug_include_in_plug_src.h
  IPlugCocoaSrc.h
	IPopupMenu.h
	Log.h
	NChanDelay.h
  )
if (WIN32)
SET (IPLUG_SRC
	${IPLUG_SRC}
	IGraphicsWin.cpp
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/IPlugVST3.def
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}../../../IPlugExamples/VMPCWDL)
elseif(APPLE)
SET (IPLUG_SRC
	${IPLUG_SRC}
	IGraphicsCarbon.cpp
	IPlugAU.r
#	IPlugCocoa.mm
)
set_source_files_properties(IPlugAU.r PROPERTIES XCODE_EXPLICIT_FILE_TYPE "Rez Sources")
endif(WIN32)

add_library(iplug ${IPLUG_SRC} ${IPLUG_HEADERS} ${IPLUG_DOC_HEADERS} )


	set_target_properties(iplug PROPERTIES COMPILE_FLAGS_RELEASE "/MD")
	set_target_properties(iplug PROPERTIES COMPILE_FLAGS_DEBUG "/MDd")


if (WIN32)
		if (BUILD_DYNAMIC)
			target_compile_options(iplug PRIVATE "/MD$<$<CONFIG:Debug>:d>")
		else()
			target_compile_options(iplug PRIVATE "/MT$<$<CONFIG:Debug>:d>")
		endif()
endif(WIN32)