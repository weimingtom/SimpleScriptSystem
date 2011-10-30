#include "config.h"

#if USE_GDI || USE_WGL
#include <windows.h>
#endif 

#ifndef BOOL
#define BOOL int
#endif 
#ifndef TRUE
#define TRUE 1
#endif 
#ifndef FALSE
#define FALSE 0
#endif 

#include <string.h>
#include "keyboard.h"

struct keyboard_buffer_struct {
	int status;
	int buffer[BUFSIZE];
	int head;
	int tail;
	BOOL reading;
	BOOL writing;
};
static struct keyboard_buffer_struct s_KBuffer;

static int KeyboardGetBufNum(int head, int tail)
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

void KeyboardInit(void)
{
    s_KBuffer.head = 0;
    s_KBuffer.tail = 0;
	memset(s_KBuffer.buffer, 0, sizeof(s_KBuffer.buffer));
}

void KeyboardRelease(void)
{
    s_KBuffer.head = 0;
    s_KBuffer.tail = 1;
    s_KBuffer.buffer[0] = -1;
}

void KeyboardChar(int Key)
{
	if (KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail) < BUFSIZE - 1)
	{
		int s = 0;
		s_KBuffer.writing = TRUE;
		s_KBuffer.buffer[s_KBuffer.tail] = ((int)Key) | (s << 8);
		if (s_KBuffer.tail == BUFSIZE - 1)
		{
			s_KBuffer.tail = 0;
		}
		else
		{
			s_KBuffer.tail++;
		}
		s_KBuffer.writing = FALSE;
	}
}

int KeyboardGetKeyboardStatus(void)
{
    int status;
    if (KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail) > 0)
    {
		/*
		MiscTrace("KeyboardGetBufNum: %d\n", 
		   KeyboardGetBufNum(s_KBuffer.head, s_KBuffer.tail));
		*/
        s_KBuffer.reading = TRUE;
        status = s_KBuffer.buffer[s_KBuffer.head];
		if (s_KBuffer.head == BUFSIZE - 1)
		{
			s_KBuffer.head = 0;
		}
		else
		{
			s_KBuffer.head++;
		}
		s_KBuffer.reading = FALSE;
        return status;
    }
    return 0;
}
