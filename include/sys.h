#ifndef SYS_H
#define SYS_H

#include "common.h"

struct buffer;
struct str_buf;

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
load_file(struct str_buf* str_buf, const char* file_name);

int
write_to_file(const char* file_name, struct str_buf buffer_str);

#endif /* SYS_H */
