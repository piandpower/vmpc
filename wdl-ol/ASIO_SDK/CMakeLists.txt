#
# ASIO (WIN32 only) library build
#

SET(ASIO_SRC
	asio.cpp
	asiodrivers.cpp
	asiolist.cpp
	iasiothiscallresolver.cpp
)

SET(ASIO_HEADERS
	asio.h
	asiodrivers.h
	asiodrvr.h
	asiolist.h
	asiosys.h
	dsound.h
	ginclude.h
	iasiodrv.h
	iasiothiscallresolver.h
	soundcard.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)


if(WIN32)
	SET(ASIO_SRC
		${ASIO_SRC}
	)
	SET(ASIO_HEADERS
		${ASIO_HEADERS}
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(asio ${ASIO_SRC} ${ASIO_HEADERS} )

