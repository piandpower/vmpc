#ifndef _IPLUG_PORTAB_H_
#define _IPLUG_PORTAB_H_
// Portable API for IPlug
//   Created by Fabien on Jul, 11th, 2014
//   This is portability header tht should ideally take place in WDL but as for now will be implemented here
//   to ease with WDL merging
//
// portable min max
#ifndef MIN
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a)>(b) ? (a) : (b))
#endif

// Add future OS dependent API or MACROS here:
#ifdef _WIN32
#elif defined __APPLE__
#elif defined __linux || defined __linux__ || defined linux
#else
#error "No OS defined!"
#endif

#endif // _IPLUG_PORTAB_H_