#include "buffer.h"
#include "sys.h"
#include "row.h"
#include "input.h"
#include "str.h"
#include "sm.h"

extern struct config config;

static struct buffer current_buffer;

struct buffer*
buffer_current(void)
{
    return &current_buffer;
}

void
buffer_delete_row(int index)
{
    struct buffer* buffer = buffer_current();
    
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
buffer_update(void)
{
    struct buffer* buffer = buffer_current();

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
buffer_append_row(int at_line, struct str_buf str)
{
    struct buffer* buffer = buffer_current();

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
buffer_serialize(void)
{
    struct buffer* buffer = buffer_current();

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
buffer_fill(struct str_buf text)
{
    struct buffer* buffer = buffer_current();

    while(1)
    {
	struct str_buf val = str_buf_split(&text, cstr("\n")); 
	if (str_buf_cmp(val, str_buf_nan))
	{
	    break;
	}
	
	buffer_append_row(buffer->num_rows, val);
	str_buf_deinit(&val);
    }
}

void
buffer_insert_row(void)
{
    struct buffer* buffer = buffer_current();

    if (buffer->cp.x == 0)
    {
        buffer_append_row(buffer->cp.y, cstr(""));
    }
    else
    {
	struct row *row = buffer->row + buffer->cp.y;
	buffer_append_row(buffer->cp.y + 1, cstrn(row->chars.data + buffer->cp.x, row->chars.size - buffer->cp.x));
	row = buffer->row + buffer->cp.y;
	row_resize(row, buffer->cp.x);
    }
    buffer->cp.y++;
    buffer->cp.x = 0;
}

void
buffer_insert_char(char c)
{
    struct buffer* buffer = buffer_current();

    int index = buffer->cp.x;
    
    if (buffer->cp.y == buffer->num_rows)
    {
	buffer_append_row(buffer->num_rows, cstr(""));
    }
    
    struct row* row = buffer->row + buffer->cp.y;
    row_insert_char(row, index, c);
    buffer->cp.x++;
    buffer->dirty++;
}

void
buffer_remove_char(void)
{
    struct buffer* buffer = buffer_current();

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
	row_append_string(&buffer->row[buffer->cp.y - 1], cstrn(row->chars.data, row->chars.size));
	buffer_delete_row(buffer->cp.y);
	buffer->cp.y--;
	buffer->dirty++;
    }
}

void
buffer_cursor_previous(void)
{
    struct buffer* buffer = buffer_current();

    struct row* row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y]; 
    
    if (buffer->cp.y != 0)
    {
	buffer->cp.y--;	    
    }

    row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];
    int rowlen = row ? row->chars.size : 0;
    if (buffer->cp.x > rowlen)
    {
	buffer->cp.x = rowlen;
    }
}

void
buffer_cursor_next(void)
{
    struct buffer* buffer = buffer_current();

    struct row* row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y]; 
    
    if (buffer->cp.y < buffer->num_rows)
    {
	buffer->cp.y++;	    
    }

    row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];
    int rowlen = row ? row->chars.size : 0;
    if (buffer->cp.x > rowlen)
    {
	buffer->cp.x = rowlen;
    }
}

void
buffer_cursor_forward(void)
{
    struct buffer* buffer = buffer_current();

    struct row* row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y]; 
    
    if (row && buffer->cp.x < row->chars.size)
    {
	buffer->cp.x++;	    	    
    }
    else if (row && buffer->cp.x == row->chars.size)
    {
	buffer->cp.y++;
	buffer->cp.x = 0;
    }

    row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];
    int rowlen = row ? row->chars.size : 0;
    if (buffer->cp.x > rowlen)
    {
	buffer->cp.x = rowlen;
    }
}

void
buffer_cursor_backward(void)
{
    struct buffer* buffer = buffer_current();

    struct row* row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];     
    
    if (buffer->cp.x != 0)
    {
	buffer->cp.x--;	    
    }
    else if (buffer->cp.y > 0)
    {
	buffer->cp.y--;
	buffer->cp.x = buffer->row[buffer->cp.y].chars.size;
    }
    
    row = (buffer->cp.y >= buffer->num_rows) ? NULL : &buffer->row[buffer->cp.y];
    int rowlen = row ? row->chars.size : 0;
    if (buffer->cp.x > rowlen)
    {
	buffer->cp.x = rowlen;
    }
}

void
buffer_open_file(const char* file_name)
{
    struct buffer* buffer = buffer_current();

    free(buffer->file_name);
    buffer->file_name = strdup(file_name);

    struct str_buf str_buf = str_buf_init(100);
    load_file(&str_buf, file_name);
    buffer_fill(str_buf);
    str_buf_deinit(&str_buf);

    buffer->dirty = 0;
}

void // TODO: NEED TO refactor
buffer_save(void)
{
    struct buffer* buffer = buffer_current();

    if (buffer->file_name == NULL)
    {
	return;
    }

    struct str_buf buffer_str = buffer_serialize();

    if (write_to_file(buffer->file_name, buffer_str))
    {
        sm_set_message("%d bytes has been saved to disk", buffer_str.size);
	buffer->dirty = 0;	
    }
    else
    {
        sm_set_message("Error: Can't save file %s", strerror(errno));       
    }

    str_buf_deinit(&buffer_str);	
}

