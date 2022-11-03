#include "buffer.h"
#include "sys.h"
#include "row.h"

extern struct config config;

void
buffer_scroll_update(struct buffer* buffer)
{
    buffer->cp.r = buffer->cp.x;

    if (buffer->cp.y < buffer->num_rows)
    {
	buffer->cp.r = row_cx_to_rx(&buffer->row[buffer->cp.y], buffer->cp.x);
    }

    if (buffer->cp.y < buffer->cp.rowoff)
    {
	buffer->cp.rowoff = buffer->cp.y;
    }

    if (buffer->cp.y >= buffer->cp.rowoff + config.screenrows)
    {
	buffer->cp.rowoff = buffer->cp.y - config.screenrows + 1;
    }

    if (buffer->cp.r < buffer->cp.coloff) {
	buffer->cp.coloff = buffer->cp.r;
    }
    
    if (buffer->cp.r >= buffer->cp.coloff + config.screencols) {
	buffer->cp.coloff = buffer->cp.r - config.screencols + 1;
    }
}

void
buffer_append_row(struct buffer* buffer, char* string, int len)
{
    buffer->row = realloc(buffer->row, sizeof(struct row) * (buffer->num_rows + 1));

    struct row *at = &buffer->row[buffer->num_rows];
    
    at->size = len;
    at->data = malloc(len + 1);
    memcpy(at->data, string, len);
    at->data[len] = '\0';
    at->rsize = 0;
    at->render = NULL;

    row_update(at);
    
    buffer->num_rows++;
    buffer->dirty++;
}

void
buffer_insert_char(struct buffer* buffer, int index, char c)
{
    struct row* row = buffer->row + buffer->cp.y;
    row_insert_char(row, index, c);
    buffer->dirty++;
}

void
buffer_remove_char(struct buffer* buffer, int index)
{
    struct row* row = buffer->row + buffer->cp.y;
    row_remove_char(row, index);
    buffer->dirty++;
}

void
buffer_del_row(struct buffer* buffer, int index)
{
    if (index < 0 || index >= buffer->num_rows)
    {
	return;
    }

    struct row* row = buffer->row + index;
    row_clean(row);
    
    memmove(row, buffer->row + index + 1, sizeof(struct row) * (buffer->num_rows - index -1));
    buffer->num_rows--;
    buffer->dirty++;
}
