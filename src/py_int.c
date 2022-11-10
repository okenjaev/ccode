#include "py_int.h"
#include "sys.h"

static int numargs=0;

static PyObject*
fme_insert_char(PyObject *self, PyObject *args)
{
    char c;
    if(!PyArg_ParseTuple(args, "c", &c))
        return NULL;
    return PyLong_FromLong(numargs);
}

static PyMethodDef fme_methods[] = {
    {"insert_char", fme_insert_char, METH_VARARGS,
     "insert char to current buffer"},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef fme_module = {
    PyModuleDef_HEAD_INIT, "fme", NULL, -1, fme_methods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_emb(void)
{
    return PyModule_Create(&fme_module);
}

void
py_run(char** argv)
{
    FILE *fp = fopen("py/init.py", "r");
    
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL)
    {
	die("py program exited");
    }

    Py_SetProgramName(program);
    Py_Initialize();

    PyRun_SimpleFile(fp, "py/init.py");
    
    if (Py_FinalizeEx() < 0)
    {
	die("py finalize ex");
    }
    
    PyMem_RawFree(program);
    fclose(fp);
}
