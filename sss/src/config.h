#pragma once

#define WIN32_LEAN_AND_MEAN

#define BUFSIZE 256

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SSS_APPNAME "SimpleScriptSystem"
#define SSS_VERSION "0.0.1"

#define USE_GDI 0
#define USE_SDL 1

#if USE_GDI
#define SSS_CLASS "SimpleScriptSystem"
#define SSS_TITLE SSS_APPNAME " GDI " SSS_VERSION
#endif

#if USE_SDL
#define WIN_BPP 32
#define NUM_COLORS	256
#define SSS_TITLE SSS_APPNAME " SDL " SSS_VERSION
#endif