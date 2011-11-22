#pragma once

//canvas object
#if USE_GDI || USE_WGL || USE_WGLES
#include <windows.h>
extern HDC CanvasInit(HWND hWnd);
#endif
#if USE_SDL
#include <SDL.h>
extern SDL_Surface* CanvasInit(void);
#endif
#if USE_DFB
#include <directfb.h>
extern IDirectFBSurface *CanvasInit(IDirectFB *dfb, char *text, int lenText);
#endif
#if USE_ANA
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};
/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;
    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};
extern void CanvasInit(struct android_app* pState, struct engine* pEngine);
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
