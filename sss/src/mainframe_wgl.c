#include "config.h"

#if USE_WGL

#include <windows.h>
#include <tchar.h>

#include "mainframe.h"
#include "canvas.h"
#include "mouse.h"
#include "keyboard.h"
#include "script.h"
#include "misc.h"

//WS_OVERLAPPEDWINDOW
#define WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU

static HWND s_hAppWnd;
static HDC s_hCanvas;

LRESULT CALLBACK WndProc(HWND hWnd, 
	UINT message, 
	WPARAM wParam, 
	LPARAM lParam)
{
	HDC hdc;
	switch(message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			/* int wmEvent = HIWORD(wParam); */
			switch (wmId)
			{
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_CREATE:
		s_hAppWnd = hWnd;
		SetWindowPos(hWnd, NULL, 0, 0, -1, -1, SWP_NOSIZE);
		s_hCanvas = CanvasInit(hWnd);
		MouseInit();
		KeyboardInit();
		break;
	
	case WM_DESTROY:
		KeyboardRelease();
		MouseRelease();
		CanvasRelease();
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc,
				ps.rcPaint.left, ps.rcPaint.top,
				ps.rcPaint.right - ps.rcPaint.left, 
				ps.rcPaint.bottom - ps.rcPaint.top,
				s_hCanvas, 
				ps.rcPaint.left, ps.rcPaint.top, 
				SRCCOPY);
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_CONTROL:
				break;
			
			case VK_ESCAPE:
				break;
			}
		}
		break;

	case WM_CHAR:
		{
			int Key = (UINT)wParam;
			MiscTrace("WM_CHAR: %d\n", Key);
			KeyboardChar(Key);
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			//MiscTrace("WM_MOUSEMOVE: %d, %d\n", point.x, point.y);
			MouseMove(point.x, point.y);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			MiscTrace("WM_LBUTTONDOWN: %d, %d\n", point.x, point.y);
			MouseLButtonDown(point.x, point.y);
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			MiscTrace("WM_LBUTTONUP: %d, %d\n", point.x, point.y);
			MouseLButtonUp(point.x, point.y);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPTSTR lpCmdLine, 
	int nCmdShow)
{
    WNDCLASSEX wcex = { 
		sizeof(WNDCLASSEX), 
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
		WndProc, 
		0, 0, 
		hInstance, 
		NULL, NULL, 
		(HBRUSH)(COLOR_WINDOW + 1), 
		NULL,
		SSS_CLASS, 
		NULL
	};
    RECT R = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	MiscAppInit();
	if(!RegisterClassEx(&wcex))
    {
		return 0;
    }
	AdjustWindowRect(&R, WINDOW_STYLE, FALSE);
    if (!(s_hAppWnd = CreateWindow(SSS_CLASS, 
		SSS_TITLE, 
		WINDOW_STYLE, 
		CW_USEDEFAULT,
		0, R.right - R.left, R.bottom - R.top, 
		NULL, NULL, hInstance, NULL)))
	{
        return 0;
    }
	ShowWindow(s_hAppWnd, nCmdShow);
	UpdateWindow(s_hAppWnd);
	/*
	do
	{
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else 
		{
			
        }
    } while(msg.message != WM_QUIT);
	or 
    while (1)
	{
        if (MainFrameGetMsg())
			break;
		CanvasLock();
		CanvasUnlock();
    }
	*/
	ScriptRun();
	CanvasRelease();
    return 0;
}

int MainFrameGetMsg(void) 
{
	MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	else 
	{
        return 0;
    }
	if (msg.message == WM_QUIT)
	{
		return 1;
	}
	return 0;
}

void MainFrameRefresh()
{
	InvalidateRect(s_hAppWnd, 0, FALSE);
}

void MainFrameResize(int w, int h)
{
	RECT rectWindow = {0};
        rectWindow.right = w;
        rectWindow.bottom = h;
	AdjustWindowRect(&rectWindow, WINDOW_STYLE, FALSE);
	SetWindowPos(s_hAppWnd, NULL, 
		0, 0, 
		rectWindow.right - rectWindow.left, 
		rectWindow.bottom - rectWindow.top, 
		SWP_NOMOVE);
	UpdateWindow(s_hAppWnd);
}

void MainFrameSetTitle(const char *str)
{
	SetWindowTextA(s_hAppWnd, str);
}

#endif //USE_WGL

