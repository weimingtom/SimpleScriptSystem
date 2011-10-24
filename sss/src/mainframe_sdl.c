#include "config.h"

#if USE_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h>

#include "mainframe.h"
#include "canvas.h"
#include "mouse.h"
#include "keyboard.h"
#include "script.h"
#include "misc.h"

static SDL_Surface *s_screen;
static SDL_Rect s_screenArea;
static int s_done;

#ifdef __MINGW32__
#undef main
#endif
int main(int argc, char *argv[])
{
	MiscAppInit();
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		MiscTrace("Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}
	MainFrameSetTitle(SSS_TITLE);
	s_screen = CanvasInit();
	MouseInit();
	KeyboardInit();
	s_screenArea.x = 0;
	s_screenArea.y = 0;
	s_screenArea.w = WINDOW_WIDTH;
	s_screenArea.h = WINDOW_HEIGHT;
	s_done = 0;
#if 0
	while (1) 
	{
		if(MainFrameGetMsg())
		{
			break;
		}
		{
			int k = 0;
			int width = WINDOW_WIDTH;
			int height = WINDOW_HEIGHT;
			CanvasLock();
			CanvasSetColor(CanvasRGB(0xff, 0, 0));
			CanvasMoveTo(0, 0);
			CanvasLineTo(width * 2, height * 2);
			for (k = height / 4; k < height / 2; k++)
			{
				CanvasSetPixel(width / 2, k, 
					CanvasRGB(0xff, 0, 0xff));
			}
			CanvasDrawLine(-width, height * 2, width * 2, -height, 
				CanvasRGB(0, 0, 0xff));
			CanvasUnlock();
			MainFrameRefresh();
		}
	}
#else
	ScriptRun();
#endif
	KeyboardRelease();
	MouseRelease();
	CanvasRelease();
	SDL_Quit();
	return 0;
}

int MainFrameGetMsg(void)
{
	SDL_Event event;
	if(!s_done && SDL_WaitEvent(&event))
	{
		switch (event.type) 
		{
		case SDL_MOUSEMOTION:
			MouseMove(event.button.x, event.button.y);
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			MouseLButtonDown(event.button.x, event.button.y);
			break;
		
		case SDL_MOUSEBUTTONUP:
			MouseLButtonUp(event.button.x, event.button.y);
			break;

		case SDL_KEYDOWN:
			KeyboardChar(event.key.keysym.sym);
			break;
		
		case SDL_QUIT:
			s_done = 1;
			break;
		
		default:
			break;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

void MainFrameRefresh(void)
{
	SDL_UpdateRects(s_screen, 1, &s_screenArea);
}

void MainFrameResize(int w, int h)
{
	/*
	 * not support
	 */
}

void MainFrameSetTitle(const char *str)
{
	SDL_WM_SetCaption(str, NULL);
}

#endif
