/*
 *  IPlugCocoaSrc.h
 *  All mac objc++ dependant resources to build within each iplug plugin/app
 *  MUST be compiled OUTSIDE the iPlug libs
 *  Used by the default IPlugCocoa.mm but could be included in your custom iplug mm files too.
 *  Created by Fabien on 1/24/14.
 *
 */

#define MM_CONCAT

#ifdef SA_API
//#include "../swell/swell-appstub.mm"
#include "main.mm"
#endif

#include "../swell/swell-misc.mm"
#include "../swell/swell-menu.mm"
#include "../swell/swell-wnd.mm"

#include "IGraphicsCocoa.mm"
#include "IGraphicsMac.mm"


#ifdef SA_API
#include "../swell/swellappmain.mm"
#endif

#include "../swell/swell-dlg.mm"
