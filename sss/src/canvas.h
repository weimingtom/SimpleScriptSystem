#pragma once

//canvas object
#if USE_GDI
#include <windows.h>
extern HDC CanvasInit(HWND hWnd);
#endif
#if USE_SDL
#include <SDL.h>
extern SDL_Surface* CanvasInit(void);
#endif
extern void CanvasRelease(void);

//lock canvas
extern void CanvasLock(void);
extern void CanvasUnlock(void);

//color 
extern void CanvasSetColor(int color);
extern int CanvasRGB(int r, int g, int b);

//draw pixel
extern void CanvasSetPixel(int x, int y, int color);
extern int CanvasGetPixel(int x, int y);

//draw line
extern void CanvasMoveTo(int x, int y);
extern void CanvasLineTo(int x, int y);
extern void CanvasDrawLine(int x1, int y1, int x2, int y2, int color);
