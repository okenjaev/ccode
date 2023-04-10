#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"
#include "hk.h"
#include "py_int.h"

static PyObject *my_callback;

void
input_init(void)
{

}

void
input_add_hotkey(char* string, PyObject* call_back)
{
    my_callback = call_back;
    sm_set_message("Loaded");
}

void
input_update(void)
{    
    fchar c = read_key();

    PyObject *result;
    result = PyObject_CallObject(my_callback, NULL);
}

void
input_deinit(void)
{

}
