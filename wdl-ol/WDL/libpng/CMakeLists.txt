#
# PNG library build
#

SET(PNG_SRC
	png.c
	pngerror.c
	pngget.c
	pngmem.c
	pngpread.c
	pngread.c
	pngrio.c
	pngrtran.c
	pngrutil.c
	pngset.c
	pngtrans.c
	pngwio.c
	pngwrite.c
	pngwtran.c
	pngwutil.c
)

SET(PNG_HEADERS
	png.h
	pnginfo.h
	pngstruct.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)


if(WIN32)
	SET(PNG_SRC
		${PNG_SRC}
	)
	SET(PNG_HEADERS
		${PNG_HEADERS}
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(png ${PNG_SRC} ${PNG_HEADERS} )

