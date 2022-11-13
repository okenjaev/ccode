#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

struct row;


struct cur_pos {
    int x;
    int y;
    int r;
    int rowoff;
    int coloff;
};

struct buffer
{
    struct cur_pos cp;
    int dirty;
    int num_rows;
    char* file_name;
    struct row *row;
};


struct buffer*
buffer_current(void);

void
buffer_deinit(struct buffer);

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


#endif /* BUFFER_H */
