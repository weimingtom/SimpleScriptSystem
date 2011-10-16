#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#if USE_GDI
#include <windows.h>
#endif

#include "misc.h"

void MiscTrace(const char *fmt, ...)
{
	char str[1000];
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
#if USE_GDI
	OutputDebugStringA(str);
#endif
#if USE_SDL
	fprintf(stderr, "%s", str);
#endif
    va_end(args);
}
