#ifndef FM_H
#define FM_H

void
fm_previous();

void
fm_next();

void
fm_forward();

void
fm_backward();

void
fm_open_file(const char* file_name);

void
fm_save();

void
fm_insert_row();

void
fm_insert_char(char c);

void
fm_remove_char();

void
fm_exit();

#endif /* FM_H */
