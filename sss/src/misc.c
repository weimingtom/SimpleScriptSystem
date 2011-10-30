#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#ifdef WIN32
#include <windows.h>
#include <crtdbg.h>
#endif

#include "misc.h"

void MiscTrace(const char *fmt, ...)
{
	char str[1000];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
#ifdef WIN32
	OutputDebugStringA(str);
#else
	fprintf(stderr, "%s", str);
#endif
    va_end(args);
}

void MiscAppInit(void)
{
#if defined(WIN32) && defined(_DEBUG) && USE_WIN_LEAK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef _CONSOLE
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#else
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif
#endif
	/* Test Memory Leak */
	/*
	malloc(10);
	*/
}

void MiscAppExit(void)
{
#if defined(WIN32) && defined(_DEBUG) && USE_WIN_LEAK
	_CrtDumpMemoryLeaks();
#endif
}