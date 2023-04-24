#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "array.h"

struct row;

struct cur_pos {
    fuint32 x;
    fuint32 y;
    fuint32 r;
    fuint32 rowoff;
    fuint32 coloff;
};

struct buffer
{
    struct cur_pos cp;
    fchar* file_name;
    darray(struct row) rows;

    fint32 dirty;
};


struct buffer*
buffer_current(void);

void
buffer_init(void);

void
buffer_deinit(struct buffer);

struct str_buf
buffer_serialize(void);

void
buffer_append_row(fuint32, struct str_buf);

void
buffer_delete_row(fuint32);

void
buffer_update(void);

void
buffer_row_append_string(void);

void
buffer_fill(struct str_buf);

void
buffer_insert_row(void);

void
buffer_insert_fchar(fchar c);

void
buffer_remove_fchar(void);

void
buffer_cursor_previous(void);

void
buffer_cursor_next(void);

void
buffer_cursor_forward(void);

void
buffer_cursor_backward(void);

void
buffer_open_file(const fchar* file_name);

void
buffer_save(void);


#endif /* BUFFER_H */
