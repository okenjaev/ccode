#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

struct row;
struct str;

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
    time_t status_message_time;
    char status_message[80];
    struct row *row;
};

#define BUFFER_INIT {.cp = {.x=0,.y=0,.r=0,.coloff = 0,.rowoff=0},	\
	    .dirty = 0,							\
	    .num_rows = 0,						\
	    .file_name = NULL,						\
	    .status_message_time = 0,					\
	    .status_message = {'\0'},					\
	    .row = NULL}

struct str_buf
buffer_serialize(const struct buffer* buffer);

void
buffer_append_row(struct buffer* buffer, int index, char* string, int len);

void
buffer_delete_row(struct buffer* buffer, int index);

void
buffer_insert_row(struct buffer* buffer);


void
buffer_cursor_update(struct buffer* buffer);

void
buffer_move_cursor(struct buffer* buffer, int key);


void
buffer_row_append_string(struct buffer* buffer);

void
buffer_insert_char(struct buffer* buffer, int index, char c);

void
buffer_remove_char(struct buffer* buffer);


#endif /* BUFFER_H */
