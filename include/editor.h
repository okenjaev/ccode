#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"

void
editor_set_status_message(const char* fmt, ...);

void
editor_init();

void
editor_update();

void
editor_open(const char* file_name);

void
editor_exit(void);

#endif /* EDITOR_H */
