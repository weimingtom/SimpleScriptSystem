#include "config.h"

#if USE_WGLES

#include <windows.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "canvas.h"

//NOTE:
#define USE_FLOAT

#ifdef USE_FLOAT

#define glF(x) x
#define glD(x) x
#define GL_F GL_FLOAT
typedef GLfloat GLf;

#else

#define glF(x) ((GLfixed)((x)*(1<<16)))
#define glD(x) glF(x)
#define GL_F GL_FIXED
typedef GLfixed GLf;
#define glClearColor glClearColorx
#define glTranslatef glTranslatex
#define glRotatef glRotatex
#define glMaterialfv glMaterialxv
#define glMaterialf	glMaterialx
#define glOrthof glOrthox
#define glScalef glScalex

#endif

GLf v[] = {
    glF(0.25f * WINDOW_WIDTH), glF(0.25f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.75f * WINDOW_WIDTH), glF(0.25f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.75f * WINDOW_WIDTH), glF(0.75f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.25f * WINDOW_WIDTH), glF(0.75f * WINDOW_HEIGHT), glF(0.0f),
};

GLf v2[] = {
    glF(0.25f * WINDOW_WIDTH), glF(0.25f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.75f * WINDOW_WIDTH), glF(0.25f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.75f * WINDOW_WIDTH), glF(0.75f * WINDOW_HEIGHT), glF(0.0f),
    glF(0.25f * WINDOW_WIDTH), glF(0.75f * WINDOW_HEIGHT), glF(0.0f),
};

GLf c[] = {
    glF(0.25f), glF(0.25f), glF(0.0f), glF(1.0f),
    glF(0.75f), glF(0.25f), glF(0.0f), glF(1.0f),
    glF(0.75f), glF(0.75f), glF(0.0f), glF(1.0f),
    glF(0.25f), glF(0.75f), glF(0.0f), glF(1.0f),
};

GLf c2[] = {
    glF(1.0f), glF(1.0f), glF(0.0f), glF(1.0f),
    glF(1.0f), glF(1.0f), glF(0.0f), glF(1.0f),
    glF(1.0f), glF(1.0f), glF(0.0f), glF(1.0f),
    glF(1.0f), glF(1.0f), glF(0.0f), glF(1.0f),
};

GLf n[] = {
    glF(0.f), glF(0.f), glF(1.0f),
    glF(0.f), glF(0.f), glF(1.0f),
    glF(0.f), glF(0.f), glF(1.0f),
    glF(0.f), glF(0.f), glF(1.0f),
};

GLf red[] = {
    glF(0.6f), glF(0.f), glF(0.0f), glF(1.0f),
};

GLf white[] = {
    glF(1.f), glF(1.f), glF(1.0f), glF(1.0f),
};

static HDC s_dc;
static HWND s_hWnd;
static int s_currentX;
static int s_currentY;
static EGLDisplay s_glesDisplay;
static EGLSurface s_glesSurface;
static EGLContext s_glesContext;
static int s_WindowWidth = 0;
static int s_WindowHeight = 0;
static int s_currentColor;

static void putPixel(int x, int y, int color)
{
	GLf vertex[3];
	GLf colors[4];
	vertex[0] = (GLf)x;
	vertex[1] = (GLf)y;
	vertex[2] = 0.0f;
	colors[0] = (unsigned char)color; //0xff0000; //red
	colors[1] = (unsigned char)(color >> 8); //green
	colors[2] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[3] = 0.0; //0xff0000; //black
	//
	glVertexPointer(3, GL_F, 0, vertex);
	glColorPointer(4, GL_F, 0, colors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawArrays(GL_POINTS, 0, 2);
}

static void putLine(int x1, int y1, int x2, int y2, int color)
{
	GLf vertex[6];
	GLf colors[8];
	vertex[0] = (GLf)x1;
	vertex[1] = (GLf)y1;
	vertex[2] = 0.0f;
	vertex[3] = (GLf)x2;
	vertex[4] = (GLf)y2;
	vertex[5] = 0.0f;
	colors[0] = (unsigned char)color; //0xff0000; //red
	colors[1] = (unsigned char)(color >> 8); //green
	colors[2] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[3] = 0.0; //0xff0000; //black
	colors[4] = (unsigned char)color; //0xff0000; //red
	colors[5] = (unsigned char)(color >> 8); //green
	colors[6] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[7] = 0.0; //0xff0000; //black
	//
	glVertexPointer(3, GL_F, 0, vertex);
	glColorPointer(4, GL_F, 0, colors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawArrays(GL_LINES, 0, 4);
}

static void test(void)
{
#if 0
    glVertexPointer(3, GL_F, 0, v);
    glColorPointer(4, GL_F, 0, c2);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisable(GL_TEXTURE_2D); 
    glDisableClientState(GL_NORMAL_ARRAY);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
#endif

#if 0
	putLine(0, 0, 400, 300, 0xFF0000);
#endif

#if 0
	{
		int i;
		for (i = 0; i < 400; i++)
		{
			putPixel(i, i, 0xff0000);
		}
	}
#endif
}

HDC CanvasInit(HWND hWnd)
{
	EGLConfig configs[10];
	EGLint matchingConfigs;	
	const EGLint configAttribs[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, EGL_DONT_CARE,
		EGL_DEPTH_SIZE, 16,
		EGL_STENCIL_SIZE, EGL_DONT_CARE,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE, EGL_NONE
	};
	RECT r;

	s_hWnd = hWnd;
	/**
	 * NOTE:Don't USE GetWindowDC(hWnd) !!!
     */
	s_dc = GetDC(hWnd);

	s_glesDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!eglInitialize(s_glesDisplay, NULL, NULL))
	{
		return FALSE;
	}
	if (!eglChooseConfig(s_glesDisplay, configAttribs, &configs[0], 10,  &matchingConfigs)) 
	{
		return FALSE;
	}
	if (matchingConfigs < 1)  
	{
		return FALSE;
	}
	s_glesSurface = eglCreateWindowSurface(s_glesDisplay, configs[0], hWnd, NULL);	
	if (!s_glesSurface) 
	{
		return FALSE;
	}
	s_glesContext = eglCreateContext(s_glesDisplay, configs[0], 0, NULL);
    if (!s_glesContext)
	{
		return FALSE;
	}
	eglMakeCurrent(s_glesDisplay, s_glesSurface, s_glesSurface, s_glesContext); 
	glClearColorx(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);  
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GetClientRect(hWnd, &r);  
	s_WindowWidth = r.right - r.left;
	s_WindowHeight = r.bottom - r.top;
	glViewport(r.left, r.top, s_WindowWidth, s_WindowHeight);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	return s_dc;
}

void CanvasRelease(void)
{
	if (s_glesDisplay)
	{
		eglMakeCurrent(s_glesDisplay, NULL, NULL, NULL);  
		if (s_glesContext) 
		{
			eglDestroyContext(s_glesDisplay, s_glesContext);
		}
		if (s_glesSurface) 
		{
			eglDestroySurface(s_glesDisplay, s_glesSurface);
		}
		eglTerminate(s_glesDisplay);
	}
    ReleaseDC(s_hWnd, s_dc);
}

void CanvasSetPixel(int x, int y, int color)
{
	putPixel(x, y, color);
}

int CanvasGetPixel(int x, int y)
{
	/* FIXME: */
	return 0;
}

void CanvasSetColor(int color)
{
	s_currentColor = color;
}

void CanvasMoveTo(int x, int y)
{
	s_currentX = x;
	s_currentY = y;
}

void CanvasLineTo(int x, int y)
{
	putLine(s_currentX, s_currentY, x, y, s_currentColor);
	s_currentX = x;
	s_currentY = y;
}

void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
	putLine(x1, y1, x2, y2, color);
}

void CanvasLock()
{
    glClearColor((float)((unsigned char)WINDOW_BGCOLOR) / (float)0xff,
		(float)((unsigned char)(WINDOW_BGCOLOR >> 8)) / (float)0xff,
		(float)((unsigned char)(WINDOW_BGCOLOR >> 16)) / (float)0xff,
		1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0, 1.0);
	//
	test();
}

void CanvasUnlock()
{
    glFlush(); /* or glFinish(); */
	eglSwapBuffers(s_glesDisplay, s_glesSurface); 
}

int CanvasRGB(int r, int g, int b)
{
	return RGB(r, g, b);
}

#endif
