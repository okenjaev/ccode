#ifndef SYS_H
#define SYS_H

void
die(const char* s);

int
get_window_size(int *rows, int *cols);

int
get_cursor_position(int *rows, int *cols);

void
disable_raw_mode();

void
enable_raw_mode();


#endif /* SYS_H */
