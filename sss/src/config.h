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
#define new  ::new(_NORMAL_BLOCK, __file__, __line__)

#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef new
#undef new
#endif
#define new DEBUG_NEW

/*
On Windows, when application start: 
_crtsetdbgflag(_crtdbg_alloc_mem_df | _crtdbg_leak_check_df);
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

#if USE_GDI || USE_WGL
#define SSS_CLASS "SimpleScriptSystem"
#define SSS_TITLE SSS_APPNAME " GDI " SSS_VERSION
#endif

#if USE_SDL
#define WIN_BPP 32
#define NUM_COLORS	256
#define SSS_TITLE SSS_APPNAME " SDL " SSS_VERSION
#endif
