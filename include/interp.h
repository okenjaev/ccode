#ifndef PY_INT_H
#define PY_INT_H

#define PY_SSIZE_T_CLEAN

#include "common.h"

void
interp_init(char* argv[]);

void
interp_deinit(void);

void
interp_call(void* func);

#endif /* PY_INT_H */
