#ifndef ROW_H
#define ROW_H

#include "common.h"
#include "str.h"

struct row
{
    struct str_buf fchars;
    struct str_buf render_fchars;
};

void
row_init(struct row*);

void
row_deinit(struct row* row);

void
row_resize(struct row* row, fint32 size);

fint32
row_cx_to_rx(const struct row* row, fint32 cx);

void
row_remove_fchar(struct row* row, fint32 index);

void
row_insert_fchar(struct row* row, fint32 index, fchar c);

void
row_append_string(struct row* row, struct str_buf str);

#endif /* ROW_H */
