#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "buffer.h"
#include "sys.h"

void
editor_init();

void
editor_move_cursor(int key);

void
editor_draw_rows(struct buffer* ab);

void
editor_refresh_screen();

int
editor_read_key();

void
editor_process_keys();

#endif /* EDITOR_H */
