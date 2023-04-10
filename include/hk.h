#ifndef HK_H
#define HK_H

#include "common.h"
#include <python3.10/Python.h>

struct fhk
{
    fchar* text;
    PyObject* call_back;
};

struct fhk
fhk_init(const fchar* text, PyObject* call_back);

#endif /* HK_H */
