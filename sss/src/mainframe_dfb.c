#include "config.h"

#if USE_DFB

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <Python.h>
#include <directfb.h>

#include "mainframe.h"
#include "script.h"
#include "mouse.h"
#include "keyboard.h"
#include "canvas.h"
#include "misc.h"

typedef struct _DeviceInfo DeviceInfo;

struct _DeviceInfo {
     DFBInputDeviceID device_id;
     DFBInputDeviceDescription desc;
     DeviceInfo *next;
};

static IDirectFB *dfb = NULL;
static IDirectFBSurface *primary = NULL;
static IDirectFBEventBuffer *buffer = NULL;

static char text[255] = {0};
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_button = 0;
static int quit = 0;

static DFBEnumerationResult enum_input_device( 
	DFBInputDeviceID device_id,
    DFBInputDeviceDescription desc,
    void *data )
{
    DeviceInfo **devices = data;
    DeviceInfo *device;
    device = malloc(sizeof(DeviceInfo));
    device->device_id = device_id;
    device->desc = desc;
    device->next = *devices;
    *devices = device;
    return DFENUM_OK;
}

/**
./df_window --dfb:quiet=info,quiet=warning,no-banner
conf.c
directfb.c
D_INFO
*/
int main (int argc, char **argv)
{
	DeviceInfo *devices = NULL;

	DFBCHECK (DirectFBInit (&argc, &argv));
	DFBCHECK (DirectFBCreate (&dfb));
	primary = CanvasInit(dfb, text, sizeof(text));
	dfb->EnumInputDevices( dfb, enum_input_device, &devices );
    DFBCHECK (dfb->CreateInputEventBuffer( dfb, DICAPS_ALL, DFB_FALSE, &buffer )); 	
	MouseInit();
	KeyboardInit();
#if 0
	while (1)
	{
		if (MainFrameGetMsg())
		{
			break;
		}
		CanvasLock();
		CanvasUnlock();
        MainFrameRefresh();
	}
#else
	ScriptRun();
#endif

    while (devices) 
    {
        DeviceInfo *next = devices->next;
        free( devices );
        devices = next;
    }
    if (buffer)
    {
		buffer->Release (buffer);
	}
	CanvasRelease();
	if (dfb)
	{
		dfb->Release (dfb);
	}
	return 23;
}

int MainFrameGetMsg(void) 
{
	DFBInputEvent event;
	while (buffer->GetEvent (buffer, DFB_EVENT(&event)) == DFB_OK)
	{
		/*
		see http://blog.csdn.net/situzhuge/article/details/6330794
		*/
		if (event.flags & DIEF_BUTTONS)
		{
			if (event.buttons & DIBM_LEFT)
			{
				mouse_button = 1;
				MouseLButtonDown(mouse_x, mouse_y);
			}
			else
			{
				mouse_button = 0;
				MouseLButtonUp(mouse_x, mouse_y);
			}
			/*
			snprintf(text, sizeof(text), "Mouse %d, %d, %d", mouse_x, mouse_y, mouse_button);
			*/ 
		}
		if (event.type == DIET_AXISMOTION) 
		{
			if (event.flags & DIEF_AXISABS) 
			{
			   switch (event.axis) 
			   {
				case DIAI_X:
					{
						mouse_x = event.axisabs;
					}
					break;
				
				case DIAI_Y:
					{
						mouse_y = event.axisabs;
					}
					break;
				}
			}
			else if (event.flags & DIEF_AXISREL) 
			{
				switch (event.axis) 
				{
				case DIAI_X:
					{
						mouse_x += event.axisrel;
					}
					break;
				
				case DIAI_Y:
					{
						mouse_y += event.axisrel;
					}
					break;
				}
			}
			MouseMove(mouse_x, mouse_y);
			/*
			snprintf(text, sizeof(text), "Mouse %d, %d, %d", mouse_x, mouse_y, mouse_button);
			*/ 
		}	
		if (event.type == DIET_KEYRELEASE)
		{
			
		}
		if (event.type == DIET_KEYPRESS)
		{
			if (event.key_id == DIKI_ESCAPE)
			{
				quit = 1;
			}
			/*
			sprintf(text, "Press %d", event.key_id);
			*/
			KeyboardChar(event.key_id);
		}
	}
	if (quit == 1)
	{
		return 1;
	}
	return 0;
}

void MainFrameRefresh()
{
	/*
	not supported
	*/
}

void MainFrameResize(int w, int h)
{
	/*
	not supported
	*/
}

void MainFrameSetTitle(const char *str)
{
	snprintf(text, sizeof(text), "%s", str);
}

#endif
