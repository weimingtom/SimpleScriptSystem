#include "config.h"

#if USE_ANA

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "canvas.h"

static struct android_app *g_state;
static struct engine *g_engine;

static int s_currentColor;
static int s_currentX;
static int s_currentY;

//see http://hi.baidu.com/fairzy/blog/item/4cd9ed2d4c3af832349bf7fc.html
static void drawPoint(GLenum mode, GLfixed *vertex, GLubyte *color, int size, int count)
{ 
	//GL_API void GL_APIENTRY glPointSize (GLfloat size);
	glPointSize(size);
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	*/
	//GL_API void GL_APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	glVertexPointer(2, GL_FIXED, 0, vertex);
	/*
	glEnableClientState(GL_COLOR_ARRAY);
	*/
	//GL_API void GL_APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);	
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
	//GL_API void GL_APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);	
    ///*GL_LINES*/ GL_POINTS
    glDrawArrays(mode, 0, count);	
}

static void putPixel(int x, int y, int color)
{
	GLfixed vertex[2] = {0}; 
	GLubyte colors[4] = {0}; 
	vertex[0] = x * 0x10000;	
	vertex[1] = y * 0x10000;
	colors[0] = (unsigned char)color; //0xff0000; //red
	colors[1] = (unsigned char)(color >> 8); //green
	colors[2] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[3] = 0; //0xff0000; //black
	drawPoint(GL_POINTS, vertex, colors, 1, 1); 
}

static void putLine(int x1, int y1, int x2, int y2, int color)
{
	GLfixed vertex[4] = {0}; 
	GLubyte colors[8] = {0}; 
	vertex[0] = x1 * 0x10000;	
	vertex[1] = y1 * 0x10000;
	vertex[2] = x2 * 0x10000;	
	vertex[3] = y2 * 0x10000;
	colors[0] = (unsigned char)color; //0xff0000; //red
	colors[1] = (unsigned char)(color >> 8); //green
	colors[2] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[3] = 0; //0xff0000; //black
	colors[4] = (unsigned char)color; //0xff0000; //red
	colors[5] = (unsigned char)(color >> 8); //green
	colors[6] = (unsigned char)(color >> 16); //0xff0000; // blue
	colors[7] = 0; //0xff0000; //black
	drawPoint(GL_LINES, vertex, colors, 1, 2); 
}

static void test(void)
{
	int i, j;
	for (i = 0; i < 255 ; i++) 
	{
		for (j = 0; j < 255; j++) 
		{
			putPixel(i, j, (i << 8) | j);
		}
	}
}

void CanvasInit(struct android_app *pState, struct engine *pEngine)
{
	g_state = pState;
	g_engine = pEngine;
	// loop waiting for stuff to do.
    g_engine->animating = 1;
}

void CanvasRelease(void)
{

}

void CanvasSetPixel(int x, int y, int color)
{
	putPixel(x, y, color);
}

int CanvasGetPixel(int x, int y)
{
	/* not supported  */
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
    if (!g_engine->animating || g_engine->display == NULL || 
		g_engine->surface == NULL) 
    {
        return;
    }
    glClearColor((float)((unsigned char)WINDOW_BGCOLOR) / (float)0xff,
		(float)((unsigned char)(WINDOW_BGCOLOR >> 8)) / (float)0xff,
		(float)((unsigned char)(WINDOW_BGCOLOR >> 16)) / (float)0xff,
		1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//test();
}

void CanvasUnlock()
{
    glFlush(); /* or glFinish(); */
    if (g_engine->display && g_engine->surface) 
    {
		eglSwapBuffers(g_engine->display, g_engine->surface);
    }
}

int CanvasRGB(int r, int g, int b)
{
	return (unsigned char)(r) | 
		((unsigned char)(g) <<  8) |
		((unsigned char)(b) << 16);
}

#endif
