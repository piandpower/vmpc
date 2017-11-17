#pragma once

#if defined(_WIN64) || defined(_WIN32)
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #ifndef WRP_NO_KEY_WRITER
    #include <rtccore.h>
  #endif
  #include <windef.h>
  #include <winbase.h>
  #include <winuser.h>
#elif defined(__APPLE__)
  #include "Carbon/Carbon.h"
#elif defined(__linux)
  #include <linux/uinput.h>
#endif
