#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "renderb.h"
#include "sys.h"
#include "buffer.h"

extern struct buffer e;

void
editor_set_status_message(const char* fmt, ...);

void
editor_init();

void
editor_move_cursor(int key);

void
editor_draw_rows(struct renderb* ab);

void
editor_refresh_screen();

int
editor_read_key();

void
editor_process_keys();

void
editor_open(const char* file_name);

#endif /* EDITOR_H */
