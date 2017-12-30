#define PLUG_MFR "Izmar"
#define PLUG_NAME "VMPCWDL"

#define PLUG_CLASS_NAME VMPCWDL

#define BUNDLE_MFR "Izmar"
#define BUNDLE_NAME "VMPCWDL"

#define PLUG_ENTRY VMPCWDL_Entry
#define PLUG_FACTORY VMPCWDL_Factory
#define PLUG_VIEW_ENTRY VMPCWDL_ViewEntry

#define PLUG_ENTRY_STR "VMPCWDL_Entry"
#define PLUG_VIEW_ENTRY_STR "VMPCWDL_ViewEntry"

#define VIEW_CLASS VMPCWDL_View
#define VIEW_CLASS_STR "VMPCWDL_View"

// Format        0xMAJR.MN.BG - in HEX! so version 10.1.5 would be 0x000A0105
#define PLUG_VER 0x00010000
#define VST3_VER_STR "1.0.0"

#define PLUG_COPYRIGHT  "Copyright 2017 Acme Inc"

#define PLUG_UNIQUE_ID 'Vmpc'
// make sure this is not the same as BUNDLE_MFR
#define PLUG_MFR_ID 'Acme'

// ProTools stuff
#if (defined(AAX_API) || defined(RTAS_API)) && !defined(_PIDS_)
  #define _PIDS_
  const int PLUG_TYPE_IDS[2] = {'PSN1', 'PSN2'};
#endif
#define PLUG_MFR_PT "Izmar\nIzmar\nAcme\n"
#define PLUG_NAME_PT "VMPCWDL\nIPPS"
#define PLUG_TYPE_PT "Effect" // valid options "None" "EQ" "Dynamics" "PitchShift" "Reverb" "Delay" "Modulation" "Harmonic" "NoiseReduction" "Dither" "SoundField" "Effect" instrument determined by PLUG _IS _INST
#define PLUG_DOES_AUDIOSUITE 0

#if (defined(AAX_API) || defined(RTAS_API)) 
#define PLUG_CHANNEL_IO "1-1 2-2"
#else
#define PLUG_CHANNEL_IO "2-10"
#endif


#define PLUG_LATENCY 0
#define PLUG_IS_INST 1

// if this is 0 RTAS can't get tempo info
#define PLUG_DOES_MIDI 1

#define PLUG_DOES_STATE_CHUNKS 0

// Unique IDs for each image resource.
#define BG_ID         	101
#define LED_GREEN_ID	102
#define LED_RED_ID		103
#define DATAWHEEL_ID	104
#define SLIDER_ID		105
#define RECKNOB_ID		106
#define VOLKNOB_ID		107

// Image resource locations for this plug.
#define BG_FN			"resources/img/bg.png"
#define LED_GREEN_FN	"resources/img/led_green.png"
#define LED_RED_FN		"resources/img/led_red.png"
#define DATAWHEEL_FN	"resources/img/datawheels.png"
#define SLIDER_FN		"resources/img/sliders.png"
#define RECKNOB_FN		"resources/img/recknobs.png"
#define VOLKNOB_FN		"resources/img/volknobs.png"

// GUI default dimensions
#define GUI_WIDTH   1298
#define GUI_HEIGHT  994
#define gui_scale 1

// on MSVC, you must define SA_API in the resource editor preprocessor macros as well as the c++ ones
#if defined(SA_API) && !defined(OS_IOS)
#include "app_wrapper/app_resource.h"
#endif

// vst3 stuff
#define MFR_URL "www.olilarkin.co.uk"
#define MFR_EMAIL "spam@me.com"
#define EFFECT_TYPE_VST3 "Instrument|Synth"

/* "Fx|Analyzer"", "Fx|Delay", "Fx|Distortion", "Fx|Dynamics", "Fx|EQ", "Fx|Filter",
"Fx", "Fx|Instrument", "Fx|InstrumentExternal", "Fx|Spatial", "Fx|Generator",
"Fx|Mastering", "Fx|Modulation", "Fx|PitchShift", "Fx|Restoration", "Fx|Reverb",
"Fx|Surround", "Fx|Tools", "Instrument", "Instrument|Drum", "Instrument|Sampler",
"Instrument|Synth", "Instrument|Synth|Sampler", "Instrument|External", "Spatial",
"Spatial|Fx", "OnlyRT", "OnlyOfflineProcess", "Mono", "Stereo",
"Surround"
*/
