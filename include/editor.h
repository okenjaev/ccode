#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"

void
editor_set_status_message(const char* fmt, ...);

void
editor_init();

void
editor_draw_update();

void
editor_input_update();

void
editor_open(const char* file_name);

#endif /* EDITOR_H */
