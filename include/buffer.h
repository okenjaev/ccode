#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

struct buffer_row
{
    int size;
    int rsize;
    char* data;
    char* render;
};

struct buffer
{
    int cx;
    int cy;
    int rx;
    int num_rows;
    int rowoff;
    int coloff;
    char* file_name;
    time_t status_message_time;
    char status_message[80];
    struct buffer_row *row;
};

#define BUFFER_INIT {.cx = 0, \
	    .cy = 0, \
	    .rx = 0, \
	    .num_rows = 0,\
	    .rowoff = 0, \
	    .coloff= 0, \
	    .file_name = NULL, \
	    .status_message_time = 0,\
	    .status_message = {'\0'}, \
	    .row = NULL}

void
buffer_append_row(struct buffer* buffer, char* string, int len);

#endif /* BUFFER_H */
