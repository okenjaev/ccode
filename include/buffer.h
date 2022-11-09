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

#define BUFFER_INIT {.cp = {.x=0,.y=0,.r=0,.coloff = 0,.rowoff=0},	\
	    .dirty = 0,							\
	    .num_rows = 0,						\
	    .file_name = NULL,						\
	    .row = NULL}

struct buffer*
buffer_current();

struct str_buf
buffer_serialize(const struct buffer*);

void
buffer_append_row(struct buffer*, int, struct str_buf);

void
buffer_delete_row(struct buffer*, int);

void
buffer_update(struct buffer*);

void
buffer_row_append_string(struct buffer*);

void
buffer_deinit(struct buffer);

void
buffer_fill(struct buffer*, struct str_buf);

#endif /* BUFFER_H */
