#
# WDL rt audio midi 
SET(RTAM_SRC
	RtAudio.cpp
	RtMidi.cpp
)

SET(RTAM_HEADERS
	RtAudio.h
	RtMidi.h
)

if(WIN32)
	SET(RTAM_SRC
		${RTAM_SRC}
	)
	SET(RTAM_HEADERS
		${RTAM_HEADERS}
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(rtam ${RTAM_SRC} ${RTAM_HEADERS} )

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS -D__WINDOWS_MM__)
  if (BUILD_DYNAMIC)
  target_compile_options(rtam PRIVATE "/MD$<$<CONFIG:Debug>:d>")
  else()
  target_compile_options(rtam PRIVATE "/MT$<$<CONFIG:Debug>:d>")
  endif()
   endif(MSVC)
endif(WIN32)
