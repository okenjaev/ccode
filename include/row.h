#ifndef ROW_H
#define ROW_H

#include "common.h"

struct row
{
    int size;
    int rsize;
    char* data;
    char* render;
};

void
row_update(struct row* row);

void
row_clean(struct row* row);

int
row_cx_to_rx(const struct row* row, int cx);

void
row_remove_char(struct row* row, int index);

void
row_insert_char(struct row* row, int index, char c);

void
row_append_string(struct row* row, char* string, int len);

#endif /* ROW_H */
