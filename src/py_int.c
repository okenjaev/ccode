#include "py_int.h"
#include "sys.h"
#include "4me.h"

static int numargs=0;

static PyObject*
insert_message(PyObject *self, PyObject *args)
{
    char* c;
    if(!PyArg_ParseTuple(args, "s", &c))
        return NULL;

    while(*c != '\0')
    {
	if (*c == '\n')
	{
	    fm_insert_row();
	}
	else
	{
	    fm_insert_char(*c);
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
	fm_previous();

    return Py_None;
}

static PyObject*
next(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	fm_next();

    return Py_None;
}

static PyObject*
forward(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	fm_forward();

    return Py_None;
}

static PyObject*
backward(PyObject* self, PyObject *args)
{
    int t = 1;
    if (!PyArg_ParseTuple(args, "|i", &t))
	return NULL;

    for(int i = 0; i < t; i++)
	fm_backward();

    return Py_None;
}

static PyObject*
quit(PyObject* self, PyObject* Py_UNUSED(args))
{
    fm_exit();
    return Py_None;
}

static PyObject*
save(PyObject* self, PyObject* Py_UNUSED(args))
{
    fm_save();
    return Py_None;
}

static PyObject*
open_file(PyObject* self, PyObject* args)
{
    char* s;
    if(!PyArg_ParseTuple(args, "s", &s))
        return NULL;

    fm_open_file(s);
    
    return Py_None;
}

static PyMethodDef fme_methods[] = {
    {"insert_message", insert_message, METH_VARARGS,
     "insert char in the current buffer"},
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
