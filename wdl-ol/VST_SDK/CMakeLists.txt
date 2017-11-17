# VST SDK

if(WIN32)
   if(MSVC)
#      add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)

SET(VST_SRC
)

SET(VST_HEADERS
	aeffect.h
	aeffectx.h
	vstfxstore.h
)
