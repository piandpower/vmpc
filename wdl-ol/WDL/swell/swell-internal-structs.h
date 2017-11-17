#ifndef _SWELL_INTERNAL_STRUCTS_H_
# define _SWELL_INTERNAL_STRUCTS_H_

# include "../ptrlist.h"

# ifdef SWELL_TARGET_OSX

#  if 0
// at some point we should enable this and use it in most SWELL APIs that call Cocoa code...
#   define SWELL_BEGIN_TRY @try {
#   define SWELL_END_TRY(x) } @catch (NSException *ex) { NSLog(@"SWELL exception in %s:%d :: %@:%@\n",__FILE__,__LINE__,[ex name], [ex reason]); x }
#  else
#   define SWELL_BEGIN_TRY
#   define SWELL_END_TRY(x)
#  endif

#  ifdef __OBJC__


#   if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_4
typedef int NSInteger;
typedef unsigned int NSUInteger;
#   endif


typedef struct OwnedWindowListRec
{
  NSWindow *hwnd;
  struct OwnedWindowListRec *_next;
} OwnedWindowListRec;

typedef struct WindowPropRec
{
  char *name; // either <64k or a strdup'd name
  void *data;
  struct WindowPropRec *_next;
} WindowPropRec;


class SWELL_ListView_Row
{
public:
  SWELL_ListView_Row() : m_param(0), m_imageidx(0), m_tmp(0) { }
  ~SWELL_ListView_Row() { m_vals.Empty(true,free); }
  WDL_PtrList<char> m_vals;
  LPARAM m_param;
  int m_imageidx;

  int m_tmp;
};

struct HTREEITEM__
{
  HTREEITEM__();
  ~HTREEITEM__();
  bool FindItem(HTREEITEM it, HTREEITEM__ **parOut, int *idxOut);

  id m_dh;

  bool m_haschildren;
  char *m_value;
  WDL_PtrList<HTREEITEM__> m_children; // only used in tree mode
  LPARAM m_param;
};






// GDI internals


// 10.4 doesn't support CoreText, so allow ATSUI if targetting 10.4 SDK
#   if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
#    ifndef __LP64__
#     define SWELL_ATSUI_TEXT_SUPPORT
#    endif
#   endif

struct HGDIOBJ__
{
  int type;

  int additional_refcnt; // refcnt of 0 means one owner (if >0, additional owners)

  // used by pen/brush
  CGColorRef color;
  int wid;
  NSImage *bitmapptr;

  NSMutableDictionary *__old_fontdict; // unused, for ABI compat
  //
  // if ATSUI used, meaning IsCoreTextSupported() returned false
  ATSUStyle atsui_font_style;

  float font_rotation;

  bool _infreelist;
  struct HGDIOBJ__ *_next;

  // if using CoreText to draw text
  void *ct_FontRef;
  char font_quality;
};

struct HDC__
{
  CGContextRef ctx;
  void *ownedData; // always use via SWELL_GetContextFrameBuffer() (which performs necessary alignment)
  HGDIOBJ__ *curpen;
  HGDIOBJ__ *curbrush;
  HGDIOBJ__ *curfont;

  NSColor *__old_nstextcol; // provided for ABI compat, but unused
  int cur_text_color_int; // text color as int

  int curbkcol;
  int curbkmode;
  float lastpos_x,lastpos_y;

  void *GLgfxctx; // optionally set
  bool _infreelist;
  struct HDC__ *_next;

  CGColorRef curtextcol; // text color as CGColor
};

#  endif // __OBJC__

// 10.4 sdk just uses "float"
#  if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_4
# ifdef __LP64__
typedef double CGFloat;
# else
typedef float CGFloat;
# endif
#  endif


# elif defined(SWELL_TARGET_GDK)

#  include <gdk/gdk.h>
#  include <gdk/gdkkeysyms.h>

# else
// generic

# endif // ifdef SWELL_TARGET_OSX

# ifndef SWELL_TARGET_OSX

#  ifdef SWELL_LICE_GDI
#   include "../lice/lice.h"
#  endif
#  include "../assocarray.h"

LRESULT SwellDialogDefaultWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct HWND__
{
  HWND__(HWND par, int wID=0, RECT *wndr=NULL, const char *label=NULL, bool visible=false, WNDPROC wndproc=NULL, DLGPROC dlgproc=NULL);
  ~HWND__(); // DO NOT USE!!! We would make this private but it breaks PtrList using it on gcc.

  // using this API prevents the HWND from being valid -- it'll still get its resources destroyed via DestroyWindow() though.
  // DestroyWindow() does cleanup, then the final Release().
  void Retain() { m_refcnt++; }
  void Release() { if (!--m_refcnt) delete this; }




  const char *m_classname;


#  ifdef SWELL_TARGET_GDK
  GdkWindow *m_oswindow;
#  endif
  char *m_title;

  HWND__ *m_children, *m_parent, *m_next, *m_prev;
  HWND__ *m_owner, *m_owned;
  RECT m_position;
  int m_id;
  int m_style, m_exstyle;
  INT_PTR m_userdata;
  WNDPROC m_wndproc;
  DLGPROC m_dlgproc;
  INT_PTR m_extra[64];
  INT_PTR m_private_data; // used by internal controls

  bool m_visible;
  bool m_hashaddestroy;
  bool m_enabled;
  bool m_wantfocus;

  int m_refcnt;

  HMENU m_menu;

  WDL_StringKeyedArray<void *> m_props;

#  ifdef SWELL_LICE_GDI
  void *m_paintctx; // temporarily set for calls to WM_PAINT

// todo:
  bool m_child_invalidated; // if a child is invalidated
  bool m_invalidated; // set to true on direct invalidate. todo RECT instead?

  LICE_IBitmap *m_backingstore; // if NULL, unused (probably only should use on top level windows, but support caching?)
#  endif
};

struct HMENU__
{
  HMENU__() { }
  ~HMENU__() { items.Empty(true,freeMenuItem); }

  WDL_PtrList<MENUITEMINFO> items;

  HMENU__ *Duplicate();
  static void freeMenuItem(void *p);

};


struct HGDIOBJ__
{
  int type;
  int additional_refcnt; // refcnt of 0 means one owner (if >0, additional owners)

  int color;
  int wid;

  struct HGDIOBJ__ *_next;
  bool _infreelist;
#  ifdef SWELL_FREETYPE
  void *fontface; // FT_Face
#  endif

};


struct HDC__
{
#  ifdef SWELL_LICE_GDI
  LICE_IBitmap *surface; // owned by context. can be (and usually is, if clipping is desired) LICE_SubBitmap
  POINT surface_offs; // offset drawing into surface by this amount

  RECT dirty_rect; // in surface coordinates, used for GetWindowDC()/GetDC()/etc
  bool dirty_rect_valid;
#  else
  void *ownedData; // for mem contexts, support a null rendering
#  endif

  HGDIOBJ__ *curpen;
  HGDIOBJ__ *curbrush;
  HGDIOBJ__ *curfont;

  int cur_text_color_int; // text color as int

  int curbkcol;
  int curbkmode;
  float lastpos_x,lastpos_y;

  struct HDC__ *_next;
  bool _infreelist;
};

# endif // ifndef SWELL_TARGET_OSX 

HDC SWELL_CreateGfxContext(void *);

// GDP internals
# define TYPE_PEN 1
# define TYPE_BRUSH 2
# define TYPE_FONT 3
# define TYPE_BITMAP 4

typedef struct
{
  void *instptr;
  void *bundleinstptr;
  int refcnt;

  int (*SWELL_dllMain)(HINSTANCE, DWORD,LPVOID); //last parm=SWELLAPI_GetFunc
  BOOL (*dllMain)(HINSTANCE, DWORD, LPVOID);
  void *lastSymbolRequested;
} SWELL_HINSTANCE;


enum
{
  INTERNAL_OBJECT_START= 0x1000001,
  INTERNAL_OBJECT_THREAD,
  INTERNAL_OBJECT_EVENT,
  INTERNAL_OBJECT_FILE,
  INTERNAL_OBJECT_EXTERNALSOCKET, // socket not owned by us
  INTERNAL_OBJECT_SOCKETEVENT,
  INTERNAL_OBJECT_NSTASK,
  INTERNAL_OBJECT_END
};

typedef struct
{
  int type; // INTERNAL_OBJECT_*
  int count; // reference count
} SWELL_InternalObjectHeader;

typedef struct
{
  SWELL_InternalObjectHeader hdr;
  DWORD (*threadProc)(LPVOID);
  void *threadParm;
  pthread_t pt;
  DWORD retv;
  bool done;
} SWELL_InternalObjectHeader_Thread;

typedef struct
{
  SWELL_InternalObjectHeader hdr;

  pthread_mutex_t mutex;
  pthread_cond_t cond;

  bool isSignal;
  bool isManualReset;

} SWELL_InternalObjectHeader_Event;


// used for both INTERNAL_OBJECT_EXTERNALSOCKET and INTERNAL_OBJECT_SOCKETEVENT.
// if EXTERNALSOCKET, socket[1] ignored and autoReset ignored.
typedef struct
{
  SWELL_InternalObjectHeader hdr;
  int socket[2];
  bool autoReset;
} SWELL_InternalObjectHeader_SocketEvent;

typedef struct
{
  SWELL_InternalObjectHeader hdr;

  FILE *fp;
} SWELL_InternalObjectHeader_File;

typedef struct
{
  SWELL_InternalObjectHeader hdr;
  void *task;
} SWELL_InternalObjectHeader_NSTask;


bool IsRightClickEmulateEnabled();

#endif // ifndef _SWELL_INTERNAL_STRUCTS_H_
