#include "config.h"

#if USE_GDI

#include <windows.h>
#include "canvas.h"

static HDC s_hBackBufferDC;
static HBITMAP s_hBitmap;
static HBITMAP s_hOldBitmap;
static HPEN s_hPen;
static HPEN s_hOldPen;
static HBRUSH s_hBrush;
static HBRUSH s_hOldBrush;

HDC CanvasInit(HWND hWnd)
{
	RECT rect;
	HDC hDC;
	GetClientRect(hWnd, &rect);
	hDC = GetDC(hWnd);
	s_hBitmap = CreateCompatibleBitmap(hDC, 
		rect.right - rect.left, 
		rect.bottom - rect.left);
	s_hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	s_hBackBufferDC = CreateCompatibleDC(hDC);
	s_hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	/*
	 * NOTE: bitmap object must be selected
	 */
	s_hOldBitmap = (HBITMAP)SelectObject(s_hBackBufferDC, s_hBitmap);
	s_hOldPen = (HPEN)SelectObject(s_hBackBufferDC, s_hPen);
	s_hOldBrush = (HBRUSH)SelectObject(s_hBackBufferDC, s_hBrush);
	ReleaseDC(hWnd, hDC);
	return s_hBackBufferDC;
}

void CanvasRelease(void)
{
	SelectObject(s_hBackBufferDC, s_hOldPen);
	SelectObject(s_hBackBufferDC, s_hOldBitmap);
	SelectObject(s_hBackBufferDC, s_hOldBrush);
	DeleteObject(s_hBackBufferDC);
	DeleteObject(s_hBitmap);
	DeleteObject(s_hPen);
	DeleteObject(s_hBrush);
}

void CanvasSetPixel(int x, int y, int color)
{
	SetPixel(s_hBackBufferDC, x, y, (COLORREF)color);
}

int CanvasGetPixel(int x, int y)
{
	return (int)GetPixel(s_hBackBufferDC, x, y);
}

void CanvasSetColor(int color)
{
	HPEN hNewPen = CreatePen(PS_SOLID, 0, color);
	SelectObject(s_hBackBufferDC, hNewPen);
	DeleteObject(s_hPen);
	s_hPen = hNewPen;
}

void CanvasMoveTo(int x, int y)
{
	MoveToEx(s_hBackBufferDC, x, y, NULL);
}

void CanvasLineTo(int x, int y)
{
	LineTo(s_hBackBufferDC, x, y);
}

void CanvasDrawLine(int x1, int y1, int x2, int y2, int color)
{
	CanvasSetColor(color);
	CanvasMoveTo(x1, y1);
	CanvasLineTo(x2, y2);
}

void CanvasLock()
{

}

void CanvasUnlock()
{

}

int CanvasRGB(int r, int g, int b)
{
	return RGB(r, g, b);
}

#endif