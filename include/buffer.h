#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

struct cur_pos {
    int x;
    int y;
    int r;
    int rowoff;
    int coloff;
};

struct buffer_row
{
    int size;
    int rsize;
    char* data;
    char* render;
};

struct buffer
{
    struct cur_pos cp;
    int num_rows;
    char* file_name;
    time_t status_message_time;
    char status_message[80];
    struct buffer_row *row;
};

#define BUFFER_INIT {.cp = {.x=0,.y=0,.r=0,.coloff = 0,.rowoff=0},	\
	    .num_rows = 0,\
	    .file_name = NULL, \
	    .status_message_time = 0,\
	    .status_message = {'\0'}, \
	    .row = NULL}

void
buffer_append_row(struct buffer* buffer, char* string, int len);

void
buffer_scroll_update(struct buffer* buffer);

#endif /* BUFFER_H */
