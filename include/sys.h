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

void
get_window_size();

void
disable_raw_mode();

void
enable_raw_mode();

void
restore();

#endif /* SYS_H */
