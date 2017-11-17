#
# LICE library build
#

SET(LICE_SRC
	lice.cpp
	lice_arc.cpp
	lice_bmp.cpp
	lice_colorspace.cpp
	lice_gif.cpp
	lice_gif_write.cpp
	lice_ico.cpp
	lice_image.cpp
	lice_jpg.cpp
	lice_jpg_write.cpp
	lice_lcf.cpp
	lice_line.cpp
	lice_lvg.cpp
	lice_palette.cpp
	lice_pcx.cpp
	lice_png.cpp
	lice_png_write.cpp
	lice_svg.cpp
	lice_texgen.cpp
	lice_text.cpp
	lice_textnew.cpp
)

SET(LICE_HEADERS
	lice.h
	lice_bezier.h
	lice_combine.h
	lice_extended.h
	lice_glbitmap.h
	lice_gl_ctx.h
	lice_lcf.h
	lice_text.h
	lice.h
	lice_bezier.h
	lice_combine.h
	lice_extended.h
	lice_lcf.h
	lice_text.h
)

if(WIN32)
   if(MSVC)
#     add_definitions(-DWIN32_LEAN_AND_MEAN)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   endif(MSVC)
endif(WIN32)

add_definitions(
    -DPNG_USE_PNGVCRD
    -DPNG_LIBPNG_SPECIALBUILD
    -D__MMX__ 
    -DPNG_HAVE_MMX_COMBINE_ROW 
    -DPNG_HAVE_MMX_READ_INTERLACE
    -DPNG_HAVE_MMX_READ_FILTER_ROW
    )

if(WIN32)
	SET(LICE_SRC
		${LICE_SRC}
        lice_gl_ctx.cpp	
        lice_glbitmap.cpp
	)
	SET(LICE_HEADERS
		${LICE_HEADERS}
	    lice_glbitmap.h
        lice_gl_ctx.h
	)

else(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(lice ${LICE_SRC} ${LICE_HEADERS} )

