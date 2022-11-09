#ifndef FM_H
#define FM_H

void
fm_buffer_cursor_move_up();

void
fm_buffer_cursor_move_down();

void
fm_buffer_cursor_move_left();

void
fm_buffer_cursor_move_right();

void
fm_buffer_open_file();

void
fm_buffer_save();

void
fm_buffer_insert_row();

void
fm_buffer_insert_char(char c);

void
fm_buffer_remove_char();

void
fm_exit();

#endif /* FM_H */
