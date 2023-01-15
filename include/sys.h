#ifndef SYS_H
#define SYS_H

#include "common.h"

struct buffer;
struct str_buf;

struct config {
    fint32 screenrows;
    fint32 screencols;  
    struct termios og_mode;    
};

void
die(const fchar* s);

void
get_window_size();

void
disable_raw_mode();

void
enable_raw_mode();

void
restore();

void
load_file(struct str_buf* str_buf, const fchar* file_name);

fint32
write_to_file(const fchar* file_name, struct str_buf buffer_str);

fchar
read_key();

#endif /* SYS_H */
