#include "config.h"

#if USE_GDI || USE_WGL
#include <windows.h>
#endif
#include <string.h>

#include "mouse.h"
#include "misc.h"

#if USE_SDL
#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#endif

#define PEN_DOWN 0xC0
#define PEN_MOVE 0x90
#define PEN_UP 0x20
#define PEN_LEAVE 0x8

struct mouse_buffer_struct {
	int status; //Is dragging
	int buffer[BUFSIZE];
	int head; //Reading position
	int tail; //Writing position
	BOOL reading; //Is reading
	BOOL writing; //Is writing
};
static struct mouse_buffer_struct s_MBuffer;

static int MouseGetBufNum(int head, int tail)
{
	if (tail > head)
	{
		return tail - head;
	}
	else if (tail < head)
	{
		return BUFSIZE - head + tail;
	} 
	else
	{
		return 0;
	}
}

void MouseInit(void)
{
    s_MBuffer.status = 0;
    s_MBuffer.writing = FALSE;
    s_MBuffer.reading = FALSE;
    s_MBuffer.head = 0;
    s_MBuffer.tail = 0;
	memset(s_MBuffer.buffer, 0, sizeof(s_MBuffer.buffer));
}

void MouseRelease(void)
{
    s_MBuffer.head = 0;
    s_MBuffer.tail = 1;
    s_MBuffer.buffer[0] = -1;
}

void MouseMove(int X, int Y)
{
	if (s_MBuffer.status == 1 && 
		MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) < BUFSIZE - 1)
	{
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_MOVE << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
	}
}

void MouseLButtonDown(int X, int Y)
{
	if (MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) < BUFSIZE - 1)
	{
		s_MBuffer.status = 1;
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_DOWN << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
	}
}

void MouseLButtonUp(int X, int Y)
{
	if (s_MBuffer.status == 1)
	{
		s_MBuffer.writing = TRUE;
		s_MBuffer.buffer[s_MBuffer.tail] = 
			((X & 0xFFF) << 12) | 
			(Y & 0xFFF) | 
			(PEN_UP << 24);
		if (s_MBuffer.tail == BUFSIZE - 1)
		{
			s_MBuffer.tail = 0;
		}
		else
		{
			s_MBuffer.tail++;
		}
		s_MBuffer.writing = FALSE;
		s_MBuffer.status = 0;
	}
}

int MouseGetMouseStatus(void)
{
    int status;
    if (MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail) > 0)
    {
		//MiscTrace("MouseGetBufNum: %d\n", 
		//	MouseGetBufNum(s_MBuffer.head, s_MBuffer.tail));
        s_MBuffer.reading = TRUE;
        status = s_MBuffer.buffer[s_MBuffer.head];
		if (s_MBuffer.head == BUFSIZE - 1)
		{
			s_MBuffer.head = 0;
		}
		else
		{
			s_MBuffer.head++;
		}
		s_MBuffer.reading = FALSE;
        return status;
    }
    return 0;
}

