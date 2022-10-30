#ifndef TERM_H
#define TERM_H

#include "common.h"
#include "editor_row.h"

struct term_config
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

    struct editor_row *row;
    struct termios og_mode;    
};

void
term_init(struct term_config* config);

int
term_get_window_size(int *rows, int *cols);

int
term_get_cursor_position(int *rows, int *cols);

void
term_disable_raw_mode();

void
term_enable_raw_mode();


#endif /* TERM_H */
