#include "py_int.h"

#include <python3.10/Python.h>

#include "sys.h"
#include "buffer.h"
#include "editor.h"

static int numargs=0;

static PyObject*
insert(PyObject *self, PyObject *args)
{
    char* c;
    if(!PyArg_ParseTuple(args, "s", &c))
        return NULL;

    while(*c != '\0')
    {
	if (*c == '\n')
	{
	    buffer_insert_row();
	}
	else
	{
	    buffer_insert_char(*c);
	}
	c++;
    }
    
    return Py_None;
}

static PyObject*
previous(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	buffer_cursor_previous();

    return Py_None;
}

static PyObject*
next(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	buffer_cursor_next();

    return Py_None;
}

static PyObject*
forward(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	buffer_cursor_forward();

    return Py_None;
}

static PyObject*
backward(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
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
    char* s;
    if(!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    buffer_open_file(s);
    
    return Py_None;
}

static PyMethodDef fme_methods[] = {
    {"insert", insert, METH_VARARGS,
     "insert string in the current buffer"},
    {"previous", previous, METH_VARARGS, "moves cursor to previous line"},
    {"next", next, METH_VARARGS, "moves cursor to next line"},
    {"forward", forward, METH_VARARGS, "moves cursor to forward at the same line"},
    {"backward", backward, METH_VARARGS, "moves cursor to backward at the same line"},
    {"quit", quit, METH_NOARGS, "exit 4me"},
    {"save", save, METH_NOARGS, "save the current buffer"},
    {"open_file", open_file, METH_VARARGS, "opens file with relative path"},
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

void
py_run(void)
{
    FILE *fp = fopen("py/init.py", "r");

    if (!fp)
    {
	return;
    }
    
    wchar_t *program = Py_DecodeLocale("./4me", NULL);
    if (program == NULL)
    {
	die("py program exited");
    }

    Py_SetProgramName(program);
    PyImport_AppendInittab("fme", &PyInit_fme);

    Py_Initialize();

    PyRun_SimpleFile(fp, "py/init.py");
    
    if (Py_FinalizeEx() < 0)
    {
	die("py finalize ex");
    }
    
    PyMem_RawFree(program);
    fclose(fp);
}
