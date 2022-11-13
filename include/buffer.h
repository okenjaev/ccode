#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

typedef void Buffer;

const char*
buffer_file_name(void);

const struct cur_pos
buffer_cur_pos(void);

const int
buffer_dirty(void);

void
buffer_deinit();

struct str_buf
buffer_serialize(void);

void
buffer_append_row(int, struct str_buf);

void
buffer_delete_row(int);

void
buffer_update(void);

void
buffer_row_append_string(void);

void
buffer_fill(struct str_buf);

void
buffer_insert_row(void);

void
buffer_insert_char(char c);

void
buffer_remove_char(void);

void
buffer_cursor_previous(void);

void
buffer_cursor_next(void);

void
buffer_cursor_forward(void);

void
buffer_cursor_backward(void);

void
buffer_open_file(const char* file_name);

void
buffer_save(void);

void
buffer_convert_to_render(struct str_buf* renderb);

#endif /* BUFFER_H */
