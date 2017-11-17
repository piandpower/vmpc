#
# WDL jnet library build
#

SET(JNET_SRC
	asyncdns.cpp
	connection.cpp
	httpget.cpp
	httpserv.cpp
	listen.cpp
	util.cpp
	webserver.cpp
)

SET(JNET_HEADERS
	asyncdns.h
	connection.h
	httpget.h
	httpserv.h
	irc_util.h
	jnetlib.h
	listen.h
	netinc.h
	util.h
	webserver.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)


if(WIN32)
	SET(JNET_SRC
		${JNET_SRC}
	)
	SET(JNET_HEADERS
		${JNET_HEADERS}
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(jnet ${JNET_SRC} ${JNET_HEADERS} )

