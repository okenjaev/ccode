#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include <python3.10/Python.h>

void
input_update(void);

void
input_init(void);

void
input_add_hotkey(char* string, PyObject* call_back);

void
input_deinit(void);

#endif /* INPUT_H */
