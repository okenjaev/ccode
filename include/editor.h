#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "buffer.h"

void
editor_set_status_message(const char* fmt, ...);

void
editor_init();

void
editor_refresh_screen();

void
editor_process_keys();

void
editor_open(const char* file_name);

#endif /* EDITOR_H */
