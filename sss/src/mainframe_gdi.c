#include "config.h"

#if USE_GDI

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "mainframe.h"
#include "script.h"
#include "mouse.h"
#include "keyboard.h"
#include "canvas.h"
#include "misc.h"

#define WINDOW_STYLE WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU
#define TIMER_ELAPSE 1

static HWND s_hAppWnd;
static HDC s_hCanvas;

static LRESULT CALLBACK WndProc(HWND hWnd, 
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
			int wmEvent = HIWORD(wParam);
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
		SetTimer(hWnd, 1, TIMER_ELAPSE, 0);
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

	case WM_TIMER:
		{
			UINT nIDEvent = (UINT)wParam;
			switch(nIDEvent)
			{
			case 1:
				break;
			}
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

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	static RECT rectWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AdjustWindowRect(&rectWindow, WINDOW_STYLE, FALSE);
    hWnd = CreateWindowA(SSS_CLASS,
		SSS_TITLE, WINDOW_STYLE, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		rectWindow.right - rectWindow.left, 
		rectWindow.bottom - rectWindow.top, 
		NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

static ATOM MyRegisterClass(HINSTANCE hInstance)
{
        WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = SSS_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
        return RegisterClassExA(&wcex);
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, 
        LPSTR lpCmdLine,
	int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MyRegisterClass(hInstance);
	if(!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	/*
	TODO: Using script engine to dispatch message,
	or the main window will have no response!!!
	As follow:
		while(1)
		{
			if(MainFrameMainLoop())
				break;
		}
	*/
	ScriptRun();
	/*
	FIXME:
	the return value of program should be : 
	(int) msg.wParam;
	*/
	return 0; 
}

int MainFrameGetMsg(void) 
{
	MSG msg;
	if(!GetMessage(&msg, 0, 0, 0))
	{
		return 1;
	}
	if(!TranslateAccelerator(msg.hwnd, NULL, &msg)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

#endif
