#ifndef SYS_H
#define SYS_H

#include "common.h"

struct buffer;

struct config {
    int screenrows;
    int screencols;  
    struct termios og_mode;    
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

void
load_file(struct buffer* buffer, const char* file_name);

#endif /* SYS_H */
