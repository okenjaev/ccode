#include "sys.h"
#include "row.h"
#include "input.h"
#include "str.h"
#include "sm.h"

extern struct config config;

typedef struct
{
    struct cur_pos cp;
    int dirty;
    int num_rows;
    char* file_name;
    struct row *row;
} Buffer;

static Buffer current_buffer;

const char*
buffer_file_name(void)
{
    return current_buffer.file_name; 
}

const struct cur_pos
buffer_cur_pos(void)
{
    return current_buffer.cp;
}

const int
buffer_dirty(void)
{
    return current_buffer.dirty;
}

void
buffer_delete_row(int index)
{
    Buffer* buffer = &current_buffer;
    
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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
buffer_deinit()
{
    free(current_buffer.file_name);
    for (int i = current_buffer.num_rows - 1; i >= 0; i--)
    {
	row_deinit(i + current_buffer.row);
    }
    free(current_buffer.row);
}

void
buffer_fill(struct str_buf text)
{
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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
    Buffer* buffer = &current_buffer;

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

void
buffer_convert_to_render(struct str_buf* renderb)
{
    const Buffer* buffer = &current_buffer;
    
    for(int y=0; y<config.screenrows; y++)
    {
	int filerow = y + buffer->cp.rowoff;
	if (filerow >= buffer->num_rows) {
	    if (buffer->num_rows == 0 && y == config.screenrows / 3)
	    {
		char welcome[80];
		int welcomelen = snprintf(welcome, sizeof(welcome),
					  "4me version %s", FORME_VERSION);
		if (welcomelen > config.screencols)
		{
		    welcomelen = config.screencols;
		}

		int padding = (config.screencols - welcomelen) / 2;
		if (padding)
		{
		    str_buf_insert_char(renderb, renderb->size, '~');
		    padding--;
		}

		while (padding--)
		{
		    str_buf_insert_char(renderb, renderb->size, ' ');
		}

		str_buf_append(renderb, cstrn(welcome, welcomelen));
	    } else {
		str_buf_insert_char(renderb, renderb->size, '~');
	    }    
	}
	else
	{
	    struct row *row = buffer->row + filerow;
	    int len = row->render_chars.size - buffer->cp.coloff;
	    if (len < 0)
	    {
		len = 0;
	    }
	    if (len > config.screencols)
	    {
		len = config.screencols;
	    }
	    
	    str_buf_append(renderb,
			   cstrn(row->render_chars.data + buffer->cp.coloff, len));
	}
	
	str_buf_append(renderb, cstrn("\x1b[K", 3));
	str_buf_append(renderb, cstrn("\r\n", 2));
    }
}
