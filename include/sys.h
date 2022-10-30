#ifndef SYS_H
#define SYS_H

#include "common.h"

struct config {
    struct termios og_mode;    
    int screenrows;
    int screencols;  
};

void
die(const char* s);

int
get_window_size();

/* int */
/* get_cursor_position(int *rows, int *cols); */

void
disable_raw_mode();

void
enable_raw_mode();


#endif /* SYS_H */
