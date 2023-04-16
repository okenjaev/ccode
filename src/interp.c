#include "interp.h"

#include <python3.10/Python.h>

#include "sys.h"
#include "buffer.h"
#include "editor.h"
#include "input.h"

static fint32 numargs=0;

static PyObject*
insert(PyObject *self, PyObject *args)
{
    fchar* c;
    if(!PyArg_ParseTuple(args, "s", &c))
        return NULL;

    while(*c)
    {
	if (*c == '\n')
	{
	    buffer_insert_row();
	}
	else
	{
	    buffer_insert_fchar(*c);
	}
	c++;
    }
    
    return Py_None;
}

static PyObject*
previous(PyObject* self, PyObject *args)
{
    fint32 t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(fint32 i = 0; i < t; i++)
	buffer_cursor_previous();

    return Py_None;
}

static PyObject*
next(PyObject* self, PyObject *args)
{
    fint32 t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(fint32 i = 0; i < t; i++)
	buffer_cursor_next();

    return Py_None;
}

static PyObject*
forward(PyObject* self, PyObject *args)
{
    fint32 t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(fint32 i = 0; i < t; i++)
	buffer_cursor_forward();

    return Py_None;
}

static PyObject*
backward(PyObject* self, PyObject *args)
{
    fint32 t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(fint32 i = 0; i < t; i++)
        buffer_cursor_backward();

    return Py_None;
}

static PyObject*
quit(PyObject* self, PyObject* Py_UNUSED(args))
{
    editor_exit();
    return Py_None;
}

static PyObject*
save(PyObject* self, PyObject* Py_UNUSED(args))
{
    buffer_save();
    return Py_None;
}

static PyObject*
open_file(PyObject* self, PyObject* args)
{
    fchar* s;
    if(!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    buffer_open_file(s);
    
    return Py_None;
}

static PyObject*
set_kbd(PyObject* self, PyObject* args)
{
    PyObject *call_back = NULL;
    if (!PyArg_ParseTuple(args, "O:set_callback", &call_back))
    {
	return NULL;
    }

    if (!PyCallable_Check(call_back))
    {
	PyErr_SetString(PyExc_TypeError, "parameter must be callable");
	return NULL;
    }
    Py_XINCREF(call_back);
    input_add_hotkey("cx", call_back);
    return Py_None;
}

static PyMethodDef fme_methods[] = {
    {"insert", insert, METH_VARARGS, "insert string in the current buffer" },
    {"previous", previous, METH_VARARGS, "moves cursor to previous line" },
    {"next", next, METH_VARARGS, "moves cursor to next line" },
    {"forward", forward, METH_VARARGS, "moves cursor to forward at the same line" },
    {"backward", backward, METH_VARARGS, "moves cursor to backward at the same line" },
    {"quit", quit, METH_NOARGS, "exit 4me" },
    {"save", save, METH_NOARGS, "save the current buffer" },
    {"open_file", open_file, METH_VARARGS, "opens file with relative path" },
    {"set_kbd", set_kbd, METH_VARARGS, "set keyboard binding" },
    {NULL, NULL, 0, NULL}
};

static PyModuleDef fme_module = {
    PyModuleDef_HEAD_INIT, "fme", NULL, -1, fme_methods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_fme(void)
{
    return PyModule_Create(&fme_module);
}

static wchar_t *program;

void
interp_init(char* argv[])
{
    FILE *fp = fopen("py/init.py", "r");

    if (!fp)
    {
	return;
    }

    program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL)
    {
	die("py program exited");
    }

    if (PyImport_AppendInittab("fme", &PyInit_fme) == -1 )
    {
	fprintf(stderr, "Error: could not fme\n");
	die("Error");
    }
    
    Py_SetProgramName(program);
    Py_Initialize();

    PyObject *pmodule = PyImport_ImportModule("fme");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module fme");
    }
    
    PyRun_SimpleFile(fp, "py/init.py");
    fclose(fp);
}


void
interp_deinit(void)
{
    if (program)
    {
	if (Py_FinalizeEx() < 0)
	{
	    die("py finalize ex");
	}

	PyMem_RawFree(program);
    }
}

void
interp_call(void* func)
{
    PyObject *result;
    result = PyObject_CallObject(func, NULL);
}

void
interp_release(void* func)
{
    Py_XDECREF((PyObject*)func);
}
