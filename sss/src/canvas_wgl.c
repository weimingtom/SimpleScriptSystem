#include "config.h"

#if USE_WGL

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "canvas.h"

static HDC s_dc;
static HGLRC s_glRC;
static HWND s_hWnd;
static int s_currentX;
static int s_currentY;

static BOOL SetupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),
	    1, /* version */
	    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	    PFD_TYPE_RGBA, /* The kind of framebuffer. RGBA or palette. */
	    32, /* Colordepth of the framebuffer. or 24 */
	    0, 0, 0, 0, 0, 0, /* cRedBits, cRedShift, cGreenBits, cGreenShift, 
		cBlueBits, cBlueShift */
	    0,  /* cAlphaBits, Zero or greater. */ 
	    0,  /* cAlphaShift, Not used. */
	    0,  /* cAccumBits, Zero or greater. */
	    0, 0, 0, 0, /* cAccumRedBits, cAccumGreenBits, cAccumBlueBits, 
		cAccumAlphaBits, Not used. */
	    24, /* Number of bits for the depthbuffer, or 32 */ 
	    8,  /* Number of bits for the stencilbuffer, or 0 */
	    0,  /* Number of Aux buffers in the framebuffer. */
	    PFD_MAIN_PLANE, /* Specifies one of the 
		following layer type values: 
		PFD_MAIN_PLANE PFD_OVERLAY_PLANE PFD_UNDERLAY_PLANE */
	    0, /* bReserved, Not used. */
	    0, 0, 0 /* dwLayerMask, dwVisibleMask, dwDamageMask, Not used. */ 
	}; 	
	int pixelformat;
	pixelformat = ChoosePixelFormat(hDC, &pfd);
	if (pixelformat == 0)
	{
		return FALSE;
	}
    if (!SetPixelFormat(hDC, pixelformat, &pfd))
    {
		return FALSE;
	}
	return TRUE;
}

HDC CanvasInit(HWND hWnd)
{
	s_hWnd = hWnd;
    s_dc = GetDC(s_hWnd);
	if(!SetupPixelFormat(s_dc))
	{
		return 0;
	}
    s_glRC = wglCreateContext(s_dc);
    if (!wglMakeCurrent(s_dc, s_glRC))
    {
		return 0;
    }
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f); 
 		glPointSize(1.0);
		glMatrixMode(GL_PROJECTION); /* or GL_MODELVIEW */
		glLoadIdentity();
		/* Orthographic projection */
		gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);
	}
	return s_dc;
}

void CanvasRelease(void)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(s_glRC);
    ReleaseDC(s_hWnd, s_dc);
}

void CanvasSetPixel(int x, int y, int color)
{
	CanvasSetColor(color);
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

int CanvasGetPixel(int x, int y)
{
	/* FIXME: */
	return 0;
}

void CanvasSetColor(int color)
{
	glColor3ub((BYTE)color, (BYTE)(color >> 8), (BYTE)(color >> 16));
}

void CanvasMoveTo(int x, int y)
{
	s_currentX = x;
	s_currentY = y;
}

void CanvasLineTo(int x, int y)
{
	glBegin(GL_LINES);
		glVertex2i(s_currentX, s_currentY);
		glVertex2i(x, y);
	glEnd();
	s_currentX = x;
	s_currentY = y;
}

void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
	CanvasSetColor(color);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void CanvasLock()
{
    wglMakeCurrent(s_dc, s_glRC);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CanvasSetColor(WINDOW_BGCOLOR);
	glRects(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void CanvasUnlock()
{
    glFlush(); /* or glFinish(); */
	SwapBuffers(s_dc);
    wglMakeCurrent(NULL, NULL);
}

int CanvasRGB(int r, int g, int b)
{
	return RGB(r, g, b);
}

#endif
