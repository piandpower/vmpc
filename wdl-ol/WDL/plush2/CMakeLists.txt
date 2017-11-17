#
# PLUSH2 library build
#

SET(PLUSH2_SRC
	pl_cam.cpp
	pl_make.cpp
	pl_math.cpp
	pl_obj.cpp
	pl_putface.cpp
	pl_read_3ds.cpp
	pl_read_cob.cpp
	pl_read_jaw.cpp
	pl_spline.cpp
)

SET(PLUSH2_HEADERS
	plush.h
	pl_pf_tex.h
)

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)

add_library(plush ${PLUSH2_SRC} ${PLUSH2_HEADERS} )

