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
    int screenrows;
    int screencols;
    int num_rows;
    int rowoff;
    int coloff;
    time_t status_message_time;
    char* file_name;
    char status_message[80];
    struct buffer_row *row;
    struct termios og_mode;    
};

void
buffer_init(struct buffer* config);

void
buffer_append_row(struct buffer* buffer, char* string, int len);

#endif /* BUFFER_H */
