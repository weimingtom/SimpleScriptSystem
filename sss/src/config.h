#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

/* ================================= */

#if defined(_MSC_VER) && defined(_DEBUG)

/*
see 
http://www.nokuno.jp/secret/program2.html
*/
#include <crtdbg.h>
#include <malloc.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
/*
#define new  ::new(_NORMAL_BLOCK, __file__, __line__)
*/

#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef free
#undef free
#endif
#define free(s) (_free_dbg(s, _NORMAL_BLOCK))

#ifdef calloc
#undef calloc
#endif
#define calloc(m, s) (_calloc_dbg(m, s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef realloc
#undef realloc
#endif
#define realloc(m, s) (_realloc_dbg(m, s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef new
#undef new
#endif
#define new DEBUG_NEW

/*
On Windows, when application start: 
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
*/
#endif

/*
FIXME: 
python 2.2.2 on win32 has memory leak
*/
#define USE_WIN_LEAK 0

/* ================================= */

#define BUFSIZE 256

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_BGCOLOR 0xCCCCCC 

#define SSS_APPNAME "SimpleScriptSystem"
#define SSS_VERSION "0.0.1"

/* GDI on Windows */
#define USE_GDI 0
/* SDL on Windows or Ubuntu */
#define USE_SDL 1
/* OpenGL on Windows */
#define USE_WGL 0 
/* DirectFB on Linux */
#define USE_DFB 0
/* Android native activity */
#define USE_ANA 0

#if USE_GDI || USE_WGL
#define SSS_CLASS "SimpleScriptSystem"
#define SSS_TITLE SSS_APPNAME " GDI " SSS_VERSION
#endif

#if USE_SDL
#define WIN_BPP 32
#define NUM_COLORS	256
#define SSS_TITLE SSS_APPNAME " SDL " SSS_VERSION
#endif

#if USE_DFB
#define DFBCHECK(x...) \
  { \
    DFBResult err = x; \
    \
    if (err != DFB_OK) \
      { \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
        DirectFBErrorFatal( #x, err ); \
      } \
  }
#endif

#if USE_ANA
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#endif
