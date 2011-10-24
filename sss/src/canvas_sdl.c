#include "config.h"

#if USE_SDL

#include <SDL.h>

#include "canvas.h"
#include "misc.h"

static SDL_Surface *s_screen;
static int s_currentColor;
static int s_currentX;
static int s_currentY;

static void CanvasClearScreen(void)
{
	CanvasLock();
	CanvasUnlock();
	SDL_UpdateRect(s_screen, 0, 0, 0, 0);
}

#if 0
static void DrawBox(int x1, int y1, int x2, int y2, int color)
{
	CanvasDrawLine(x1, y1, x2, y1, color);
	CanvasDrawLine(x2, y1, x2, y2, color);
	CanvasDrawLine(x2, y2, x1, y2, color);
	CanvasDrawLine(x1, y2, x1, y1, color);
}
#endif

SDL_Surface* CanvasInit(void)
{
	s_screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, WIN_BPP, SDL_SWSURFACE);
	if (s_screen == NULL) 
	{
		MiscTrace("Couldn't set display mode: %s\n", SDL_GetError());
		exit(4);
	}
	CanvasClearScreen();
	return s_screen;
}

void CanvasRelease(void)
{
	SDL_FreeSurface(s_screen);
	s_screen = NULL;
}

#if 0
static void clear()
{
	Uint8 *buffer;
    buffer = (Uint8 *)s_screen->pixels;
	memset(buffer, 0, 
		(s_screen->h) * (s_screen->w) * (s_screen->format->BytesPerPixel));
}
#endif

void CanvasLock(void)
{
	static SDL_Rect area = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	if(SDL_MUSTLOCK(s_screen))
	{
		if(SDL_LockSurface(s_screen) < 0)
		{
			MiscTrace("Couldn't lock display surface: %s\n", SDL_GetError());
			exit(3);
		}
	}
	SDL_FillRect(s_screen, &area, 
		SDL_MapRGB(s_screen->format, 
		(Uint8)WINDOW_BGCOLOR, 
		(Uint8)(WINDOW_BGCOLOR >> 8), 
		(Uint8)(WINDOW_BGCOLOR >> 16)));
}

void CanvasUnlock(void)
{
	if (SDL_MUSTLOCK(s_screen))
	{
		SDL_UnlockSurface(s_screen);
	}
}

void CanvasSetPixel(int x, int y, int color)
{
	Uint32 *bufp;
	if(x < 0 || x >= s_screen->w || y < 0 || y >= s_screen->h) 
		return;
	bufp = (Uint32 *)s_screen->pixels + y * s_screen->pitch / 4 + x;
	*bufp = color;
}

int CanvasGetPixel(int x, int y)
{
	Uint32 color = 0;
	Uint32 *bufp;
	bufp = (Uint32 *)s_screen->pixels + y * s_screen->pitch / 4 + x;
	color = *bufp;
	return color;
}

/*
 * Bresenham's line algorithm
 * e'=2*e*dx.
 */
void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
    int x, y, ax, ay, sx, sy;
    ax = ((x2 - x1 > 0) ? (x2 - x1) : (x1 - x2)) << 1;
    ay = ((y2 - y1 > 0) ? (y2 - y1) : (y1 - y2)) << 1;
    sx = (x2 - x1 >= 0) ? 1 : -1;
    sy = (y2 - y1 >= 0) ? 1 : -1;
    x = x1;
    y = y1;
    if (ax > ay)
    {
        int d = ay - (ax >> 1);
        while (x != x2)
        {
            CanvasSetPixel(x, y, color);
            if (d > 0 || (d == 0 && sx == 1))
            {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    }
    else
    {
        int d = ax - (ay >> 1);
        while (y != y2)
        {
            CanvasSetPixel(x, y, color);
            if (d > 0 || (d == 0 && sy == 1))
            {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
    CanvasSetPixel(x, y, color);
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
	CanvasDrawLine(s_currentX, s_currentY, x, y, s_currentColor);
	s_currentX = x;
	s_currentY = y;
}

int CanvasRGB(int r, int g, int b)
{
	return SDL_MapRGB(s_screen->format, (Uint8)r, (Uint8)g, (Uint8)b);
}

#endif
