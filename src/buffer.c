#include "buffer.h"
#include "sys.h"
#include "row.h"
#include "input.h"
#include "str.h"

extern struct config config;

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
buffer_cursor_update(struct buffer* buffer)
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
buffer_append_row(struct buffer* buffer, int at_line, char* string, int len)
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
    row_append_string(at, string, len);
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
buffer_insert_char(struct buffer* buffer, int index, char c)
{
    if (buffer->cp.y == buffer->num_rows)
    {
	buffer_append_row(buffer, buffer->num_rows, "", 0);
    }
    
    struct row* row = buffer->row + buffer->cp.y;
    row_insert_char(row, index, c);
    buffer->cp.x++;
    buffer->dirty++;
}

void
buffer_insert_row(struct buffer* buffer)
{
    if (buffer->cp.x == 0)
    {
        buffer_append_row(buffer, buffer->cp.y, "", 0);
    }
    else
    {
	struct row *row = buffer->row + buffer->cp.y;
	buffer_append_row(buffer, buffer->cp.y + 1, row->chars.data + buffer->cp.x, row->chars.size - buffer->cp.x);
	row = buffer->row + buffer->cp.y;
	row_resize(row, buffer->cp.x);
    }
    buffer->cp.y++;
    buffer->cp.x = 0;
}

// TODO: remove int key and just receive the x and y
void
buffer_move_cursor(struct buffer* buffer, int key)
{
    struct row* row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y]; 
    
    switch (key)
    {
    case ARROW_LEFT:
	if (buffer->cp.x != 0)
	{
	    buffer->cp.x--;	    
	}
	else if (buffer->cp.y > 0)
	{
	    buffer->cp.y--;
	    buffer->cp.x = buffer->row[buffer->cp.y].chars.size;
	}
	break;
    case ARROW_RIGHT:
	if (row && buffer->cp.x < row->chars.size)
	{
	    buffer->cp.x++;	    	    
	}
	else if (row && buffer->cp.x == row->chars.size)
	{
	    buffer->cp.y++;
	    buffer->cp.x = 0;
	}
	break;
    case ARROW_UP:
	if (buffer->cp.y != 0)
	{
	    buffer->cp.y--;	    
	}
	break;
    case ARROW_DOWN:
	if (buffer->cp.y < buffer->num_rows)
	{
	    buffer->cp.y++;	    
	}
	break;
    }

    row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];
    int rowlen = row ? row->chars.size : 0;
    if (buffer->cp.x > rowlen)
    {
	buffer->cp.x = rowlen;
    }
}

void
buffer_remove_char(struct buffer* buffer)
{
    if (buffer->cp.y == buffer->num_rows)
    {
	return;
    }

    if (buffer->cp.x == 0 && buffer->cp.y == 0)
    {
	return;
    }

    struct row *row = buffer->row + buffer->cp.y;
    
    if (buffer->cp.x > 0)
    {
	struct row* row = buffer->row + buffer->cp.y;
	row_remove_char(row, buffer->cp.x - 1);
	buffer->cp.x--;
	buffer->dirty++;
    }
    else
    {
	buffer->cp.x = buffer->row[buffer->cp.y - 1].chars.size;
	row_append_string(&buffer->row[buffer->cp.y - 1], row->chars.data, row->chars.size);
	buffer_delete_row(buffer, buffer->cp.y);
	buffer->cp.y--;
	buffer->dirty++;
    }
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
buffer_fill(struct buffer* buffer, struct str text)
{
    while(1)
    {
	struct str val = str_split(&text, cstr("\n")); 
	if (val.size == -1)
	{
	    break;
	}
	
	buffer_append_row(buffer, buffer->num_rows, val.data, val.size);
	str_deinit(&val);
    }
}
