#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"

void
editor_set_status_message(const fchar* fmt, ...);

void
editor_init(char* argv[]);

void
editor_update();

void
editor_open(const fchar* file_name);

void
editor_exit(void);

#endif /* EDITOR_H */
