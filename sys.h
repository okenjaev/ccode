#ifndef SYS_H
#define SYS_H

#include "common.h"

struct term_config
{
    int cx;
    int cy;
    int screenrows;
    int screencols;
    struct termios og_mode;    
};

void
term_die(const char* s);

int
term_get_window_size(int *rows, int *cols);

int
term_get_cursor_position(int *rows, int *cols);

void
term_disable_raw_mode();

void
term_enable_raw_mode();


#endif /* SYS_H */
