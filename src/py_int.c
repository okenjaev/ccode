#include "py_int.h"
#include "sys.h"
#include "4me.h"

static int numargs=0;

static PyObject*
insert_char(PyObject *self, PyObject *args)
{
    char* c;
    if(!PyArg_ParseTuple(args, "s", &c))
        return NULL;

    fm_insert_char(*c);
    
    return PyLong_FromLong(1);
}

static PyMethodDef fme_methods[] = {
    {"insert_char", insert_char, METH_VARARGS,
     "insert char to current buffer"},
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
