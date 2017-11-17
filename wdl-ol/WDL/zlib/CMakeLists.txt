#
# ZLIB library build
#

SET(ZLIB_SRC
	adler32.c
	compress.c
	crc32.c
	deflate.c
	gzclose.c
	gzlib.c
	gzread.c
	gzwrite.c
	infback.c
	inffast.c
	inflate.c
	inftrees.c
	ioapi.c
	trees.c
	uncompr.c
	zip.c
	zutil.c
)

SET(ZLIB_HEADERS
	crc32.h
	deflate.h
	gzguts.h
	inffast.h
	inffixed.h
	inflate.h
	inftrees.h
	ioapi.h
	trees.h
	zip.h
	zlib.h
	zutil.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)


if(WIN32)
	SET(ZLIB_SRC
		${ZLIB_SRC}
	)
	SET(ZLIB_HEADERS
		${ZLIB_HEADERS}
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(zlib ${ZLIB_SRC} ${ZLIB_HEADERS} )

