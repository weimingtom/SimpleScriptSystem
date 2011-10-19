#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

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
