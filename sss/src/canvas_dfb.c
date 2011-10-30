#include "config.h"

#if USE_DFB

#include <directfb.h>

#include "canvas.h"

static IDirectFBSurface *s_primary = NULL;
static int s_screen_width = 0;
static int s_screen_height = 0;
static char *s_text = 0;
static int s_lenText = 0;
static IDirectFBFont *s_font = NULL;
static DFBSurfaceDescription s_dsc;
static DFBFontDescription s_font_dsc;

static int s_currentColor;
static int s_currentX;
static int s_currentY;

IDirectFBSurface *CanvasInit(IDirectFB *dfb, char *text, int lenText)
{	
	s_dsc.flags = DSDESC_CAPS;
	s_dsc.caps = DSCAPS_PRIMARY | DSCAPS_FLIPPING;
	
	DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
	DFBCHECK (dfb->CreateSurface( dfb, &s_dsc, &s_primary ));
	DFBCHECK (s_primary->GetSize (s_primary, &s_screen_width, &s_screen_height));
	DFBCHECK (s_primary->FillRectangle (s_primary, 0, 0, s_screen_width, s_screen_height));
	//DFBCHECK (primary->SetBlittingFlags (primary, DSBLIT_BLEND_ALPHACHANNEL));

	s_text = text;
	s_lenText = lenText;
	s_font_dsc.flags = DFDESC_HEIGHT;
	s_font_dsc.height = 16;
	DFBCHECK (dfb->CreateFont (dfb, "decker.ttf", &s_font_dsc, &s_font));
	snprintf(s_text, s_lenText, "%s", "Simple Script System 0.0.1");

	return s_primary;
}

void CanvasRelease(void)
{
	if (s_font)
	{
		s_font->Release (s_font);
	}
	if (s_primary)
	{
		s_primary->Release (s_primary);
	}
}

void CanvasSetPixel(int x, int y, int color)
{
	DFBCHECK (s_primary->SetColor (s_primary, 
		(unsigned char)color, (unsigned char)(color >> 8), (unsigned char)(color >> 16), 0xFF));
	DFBCHECK (s_primary->DrawLine (s_primary, x, y, x, y));
}

int CanvasGetPixel(int x, int y)
{
	/*
	not supported
	*/
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

void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
	DFBCHECK (s_primary->SetColor (s_primary, 
		(unsigned char)color, (unsigned char)(color >> 8), (unsigned char)(color >> 16), 0xFF));
	DFBCHECK (s_primary->DrawLine (s_primary, x1, y1, x2, y2));
}

void CanvasLock()
{
	DFBCHECK (s_primary->SetColor (s_primary, 0xcc, 0xcc, 0xcc, 0xff));
	DFBCHECK (s_primary->FillRectangle (s_primary, 0, 0, s_screen_width, s_screen_height));
	/*
	DFBCHECK (s_primary->SetColor (s_primary, 0x80, 0x80, 0xff, 0xff));
	DFBCHECK (s_primary->DrawLine (s_primary, 0, (s_screen_height / 2), s_screen_width - 1, (s_screen_height / 2) ));
	*/
}

void CanvasUnlock()
{
	DFBCHECK (s_primary->SetColor (s_primary, 0x80, 0x0, 0x20, 0xFF));
	DFBCHECK (s_primary->SetFont (s_primary, s_font));
	DFBCHECK (s_primary->DrawString (s_primary, s_text, -1, 0, 0, DSTF_LEFT | DSTF_TOP));

	DFBCHECK (s_primary->Flip (s_primary, NULL, DSFLIP_WAITFORSYNC));
}

int CanvasRGB(int r, int g, int b)
{
	return (unsigned char)(r) | 
		((unsigned char)(g) <<  8) |
		((unsigned char)(b) << 16);
}

#endif

