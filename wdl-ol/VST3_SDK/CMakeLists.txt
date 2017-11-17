# VST3 SDK
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(VST3_SRC
    "base/source/baseiids.cpp"
    "base/source/classfactory.cpp"
    "base/source/fatomic.cpp"
    "base/source/fbitset.cpp"
    "base/source/fbuffer.cpp"
    "base/source/fcpu.cpp"
    "base/source/fdebug.cpp"
    "base/source/fdynlib.cpp"
    "base/source/finitializer.cpp"
    "base/source/fmemory.cpp"
    "base/source/fobject.cpp"
    "base/source/fpoint.cpp"
    "base/source/frect.cpp"
    "base/source/fstreamer.cpp"
    "base/source/fstring.cpp"
    "base/source/fthread.cpp"
    "base/source/timer.cpp"
    "base/source/updatehandler.cpp"

    "pluginterfaces/base/conststringtable.cpp"
    "pluginterfaces/base/funknown.cpp"
    "pluginterfaces/base/ustring.cpp"
    "public.sdk/source/common/pluginview.cpp"
    "public.sdk/source/main/pluginfactoryvst3.cpp"
    "public.sdk/source/vst/vstaudioeffect.cpp"
    "public.sdk/source/vst/vstbus.cpp"
    "public.sdk/source/vst/vstcomponent.cpp"
    "public.sdk/source/vst/vstcomponentbase.cpp"
    "public.sdk/source/vst/vstinitiids.cpp"
    "public.sdk/source/vst/vstparameters.cpp"
    "public.sdk/source/vst/vstsinglecomponenteffect.cpp"
)

SET(VST3_HEADERS
    "pluginterfaces/base/falignpop.h"
    "pluginterfaces/base/falignpush.h"
    "pluginterfaces/base/fplatform.h"
    "pluginterfaces/base/ftypes.h"
    "pluginterfaces/base/funknown.h"
    "pluginterfaces/base/ipluginbase.h"
    "pluginterfaces/base/keycodes.h"
    "pluginterfaces/base/ustring.h"
    "pluginterfaces/gui/iplugview.h"
    "pluginterfaces/vst/ivstattributes.h"
    "pluginterfaces/vst/ivstaudioprocessor.h"
    "pluginterfaces/vst/ivstcomponent.h"
    "pluginterfaces/vst/ivsteditcontroller.h"
    "pluginterfaces/vst/ivstevents.h"
    "pluginterfaces/vst/ivsthostapplication.h"
    "pluginterfaces/vst/ivstmessage.h"
    "pluginterfaces/vst/ivstmidicontrollers.h"
    "pluginterfaces/vst/ivstparameterchanges.h"
    "pluginterfaces/vst/ivstprocesscontext.h"
    "pluginterfaces/vst/ivstunits.h"
    "pluginterfaces/vst/vstpresetkeys.h"
    "pluginterfaces/vst/vsttypes.h"
    "public.sdk/source/common/pluginview.h"
    "public.sdk/source/main/pluginfactoryvst3.h"
    "public.sdk/source/vst/vstaudioeffect.h"
    "public.sdk/source/vst/vstbus.h"
    "public.sdk/source/vst/vstcomponent.h"
    "public.sdk/source/vst/vstcomponentbase.h"
    "public.sdk/source/vst/vsteditcontroller.h"
    "public.sdk/source/vst/vstparameters.h"
    "public.sdk/source/vst/vstsinglecomponenteffect.h"
)

if(WIN32)
	SET(VST3_SRC ${VST3_SRC} 
	"public.sdk/source/main/dllmain.cpp"
	)
elseif(APPLE)
	SET(VST3_SRC ${VST3_SRC} 
	"public.sdk/source/main/macexport.exp"
	"public.sdk/source/main/macmain.cpp"
	)

	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(WIN32)

add_library(vst3 ${VST3_SRC} ${VST3_HEADERS} )

