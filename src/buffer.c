#include "buffer.h"
#include "sys.h"
#include "row.h"
#include "input.h"
#include "str.h"

extern struct config config;

static struct buffer current_buffer = BUFFER_INIT;

struct buffer*
buffer_current()
{
    return &current_buffer;
}

void
buffer_delete_row(struct buffer* buffer, int index)
{
    if (index < 0 || index >= buffer->num_rows)
    {
	return;
    }
    
    struct row* row = buffer->row + index;
    row_deinit(row);    
    memmove(row, row + 1, sizeof(struct row) * (buffer->num_rows - index -1));
    buffer->num_rows--;
    buffer->dirty++;
}

void
buffer_update(struct buffer* buffer)
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
buffer_append_row(struct buffer* buffer, int at_line, struct str_buf str)
{
    if (at_line < 0 || at_line > buffer->num_rows)
    {
	return;	
    }

    buffer->row = realloc(buffer->row,
			  sizeof(struct row) * (buffer->num_rows + 1));
    
    memmove(buffer->row + at_line + 1,
	    buffer->row + at_line,
	    sizeof(struct row) * (buffer->num_rows - at_line));

    struct row *at = buffer->row + at_line;
    row_init(at);
    row_append_string(at, str);
    buffer->num_rows++;
    buffer->dirty++;
}

struct str_buf
buffer_serialize(const struct buffer* buffer)
{
    struct str_buf res = str_buf_init(50);
    
    for (int i = 0; i < buffer->num_rows; i++)
    {
	struct row* cur_row = buffer->row + i;
	str_buf_append(&res, cur_row->chars);
	str_buf_insert_char(&res, res.size, '\n');
    }
    
    return res;
}


void
buffer_deinit(struct buffer buffer)
{
    free(buffer.file_name);
    for (int i = buffer.num_rows - 1; i >= 0; i--)
    {
	row_deinit(i + buffer.row);
    }
    free(buffer.row);
}

void
buffer_fill(struct buffer* buffer, struct str_buf text)
{
    while(1)
    {
	struct str_buf val = str_buf_split(&text, cstr("\n")); 
	if (str_buf_cmp(val, str_buf_nan))
	{
	    break;
	}
	
	buffer_append_row(buffer, buffer->num_rows, val);
	str_buf_deinit(&val);
    }
}
