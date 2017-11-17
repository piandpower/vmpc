#
# EEL library build
#

SET(EEL_SRC
	nseel-caltab.c
	nseel-cfunc.c
	nseel-compiler.c
	nseel-eval.c
	nseel-lextab.c
	nseel-ram.c
	nseel-yylex.c
)

SET(EEL_HEADERS
	eel_files.h
	eel_strings.h
	glue_port.h
	ns-eel-addfuncs.h
	ns-eel-int.h
	ns-eel.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)


if(WIN32)
	SET(EEL_SRC
		${EEL_SRC}
	)
else(WIN32)
	SET(EEL_SRC
		${EEL_SRC}
	)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(eel ${EEL_SRC} ${EEL_HEADERS} )

