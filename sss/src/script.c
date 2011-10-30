#include "config.h"

#include <Python.h>
#if USE_GDI
#include <windows.h>
#endif

#include "script.h"
#include "mainframe.h"
#include "canvas.h"
#include "misc.h"
#include "mouse.h"
#include "keyboard.h"

static PyObject* trace(PyObject *self, PyObject *args)
{
    char* input;
    if (!PyArg_ParseTuple(args, "s", &input))
	{
		return NULL;
	}
	MiscTrace("%s\n", input);
	return PyInt_FromLong(0);
}

static PyObject* moveTo(PyObject *self, PyObject *args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
	{
		return NULL;
	}
	CanvasMoveTo(x, y);
	return PyInt_FromLong(0);
}

static PyObject* lineTo(PyObject *self, PyObject *args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
	{
		return NULL;
	}
	CanvasLineTo(x, y);
	return PyInt_FromLong(0);
}

static PyObject* setColor(PyObject *self, PyObject *args)
{
    int color;
    if (!PyArg_ParseTuple(args, "i", &color))
	{
		return NULL;
	}
	CanvasSetColor(color);
	return PyInt_FromLong(0);
}

static PyObject* setPixel(PyObject *self, PyObject *args)
{
    int x, y, color;
    if (!PyArg_ParseTuple(args, "iii", &x, &y, &color))
	{
		return NULL;
	}
	CanvasSetPixel(x, y, color);
	return PyInt_FromLong(0);
}

static PyObject* getPixel(PyObject *self, PyObject *args)
{
    int x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
	{
		return NULL;
	}
	return PyInt_FromLong(CanvasGetPixel(x, y));
}

static PyObject* drawLine(PyObject *self, PyObject *args)
{
    int x1, y1, x2, y2, color;
    if (!PyArg_ParseTuple(args, "iiiii", &x1, &y1, &x2, &y2, &color))
	{
		return NULL;
	}
	CanvasDrawLine(x1, y1, x2, y2, color);
	return PyInt_FromLong(0);
}

static PyObject* getMsg(PyObject *self, PyObject *args)
{
	return PyInt_FromLong(MainFrameGetMsg());
}

static PyObject* refresh(PyObject *self, PyObject *args)
{
	MainFrameRefresh();
	return PyInt_FromLong(0);
}

static PyObject* resize(PyObject *self, PyObject *args)
{
    int w, h;
    if (!PyArg_ParseTuple(args, "ii", &w, &h))
	{
		return NULL;
	}
	MainFrameResize(w, h);
	return PyInt_FromLong(0);
}

static PyObject* getMouseStatus(PyObject *self, PyObject *args)
{
	return PyInt_FromLong(MouseGetMouseStatus());
}

static PyObject* getKeyboardStatus(PyObject *self, PyObject *args)
{
	return PyInt_FromLong(KeyboardGetKeyboardStatus());
}

static PyObject* setTitle(PyObject *self, PyObject *args)
{
    char* input;
    if (!PyArg_ParseTuple(args, "s", &input))
	{
		return NULL;
	}
	MainFrameSetTitle(input);
	return PyInt_FromLong(0);
}

static PyObject* lock(PyObject *self, PyObject *args)
{
	CanvasLock();
	return PyInt_FromLong(0);
}

static PyObject* unlock(PyObject *self, PyObject *args)
{
	CanvasUnlock();
	return PyInt_FromLong(0);
}

static PyObject* rgb(PyObject *self, PyObject *args)
{
    int r, g, b;
    if (!PyArg_ParseTuple(args, "iii", &r, &g, &b))
	{
		return NULL;
	}
	return PyInt_FromLong(CanvasRGB(r, g, b));
}

static int import(void)
{
	int ret;
	PyObject *m;
	static PyMethodDef methods[] = {
		{"trace", (PyCFunction)trace, METH_VARARGS, "Output debug info for debugging"},
		{"getMsg", (PyCFunction)getMsg, METH_NOARGS, "Windows get message loop"},
		{"moveTo", (PyCFunction)moveTo, METH_VARARGS, "Move to Point"},
		{"lineTo", (PyCFunction)lineTo, METH_VARARGS, "Line to point"},
		{"setColor", (PyCFunction)setColor, METH_VARARGS, "Set forground color"},
		{"refresh", (PyCFunction)refresh, METH_NOARGS, "Refresh screen"},
		{"setPixel", (PyCFunction)setPixel, METH_VARARGS, "Set pixel's color"},
		{"getPixel", (PyCFunction)getPixel, METH_VARARGS, "Get pixel's color"},
		{"drawLine", (PyCFunction)drawLine, METH_VARARGS, "Draw a line"},
		{"resize", (PyCFunction)resize, METH_VARARGS, "Resize window"},
		{"getMouseStatus", (PyCFunction)getMouseStatus, METH_NOARGS, "Get mouse status"},
		{"getKeyboardStatus", (PyCFunction)getKeyboardStatus, METH_NOARGS, "Get mouse status"},
		{"setTitle", (PyCFunction)setTitle, METH_VARARGS, "Set window's title"},
		{"lock", (PyCFunction)lock, METH_NOARGS, "lock screen"},
		{"unlock", (PyCFunction)unlock, METH_NOARGS, "unlock screen"},
		{"rgb", (PyCFunction)rgb, METH_VARARGS, "from rgb to int"},
		{NULL, NULL}
	};
	PyImport_AddModule("SSS");
	m = Py_InitModule("SSS", methods);
	PyModule_AddStringConstant(m, "SSS_VERSION", SSS_VERSION);
	ret = PyRun_SimpleString(
		"import SSS\n"
		"SSS.trace(\"Script environment is initializing...\")\n"
		"SSS.trace(\"SSS_VERSION is %s\" % SSS.SSS_VERSION)\n"
	);
	if(ret == -1)
	{
		MiscTrace("error when import\n");
		return 0;
	}
	return 1;
}

static void startup(void)
{
	int ret;
	ret = PyRun_SimpleString(
#if USE_DFB
		"width = 640\n"
		"height = 480\n"
#else
		"width = 800\n"
		"height = 600\n"
#endif
		"SSS.resize(width, height)\n"
		"while 1:\n"
		"	if SSS.getMsg():\n"
		"		break\n"
		"	SSS.lock()\n"
		"	SSS.setColor(SSS.rgb(0xff, 0, 0))\n"
		"	SSS.moveTo(0, 0)\n"
		"	SSS.lineTo(width * 2, height * 2)\n"
		"	for k in range(height / 4, height / 2):"
		"		SSS.setPixel(width / 2, k, SSS.rgb(0xff, 0, 0xff))\n"
		"	i = SSS.getPixel(width / 4, height / 2)\n"
		"	SSS.drawLine(-width, height * 2, width * 2, -height, SSS.rgb(0, 0, 0xff))\n"
		"	SSS.unlock()\n"
		"	ms = SSS.getMouseStatus()\n"
		"	if ms != 0:\n"
		"		SSS.trace(\"mouse status : %d\" % ms)\n"
		"		SSS.setTitle(\"mouse status : %d\" % ms)\n"
		"	ks = SSS.getKeyboardStatus()\n"
		"	if ks != 0:\n"
		"		SSS.trace(\"keyboard status : %d\" % ks)\n"
		"		SSS.setTitle(\"keyboard status : %d\" % ks)\n"
		"	SSS.refresh()\n"
	);
	if(ret == -1)
	{
		MiscTrace("error when startup\n");
	}
}

//NOTE:This function is in WinMain (Main Thread)
int ScriptRun(void)
{
	Py_SetProgramName(SSS_APPNAME);
	Py_Initialize();
	if (import())
	{
		startup();
	}
	Py_Finalize();
	return 0;
}



