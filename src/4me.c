#include "4me.h"
#include "buffer.h"
#include "row.h"
#include "sys.h"
#include "sm.h"

void
fm_previous()
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
fm_next()
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
fm_forward()
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
fm_backward()
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
fm_open_file(const char* file_name)
{
    struct buffer* buffer = buffer_current();

    free(buffer->file_name);
    buffer->file_name = strdup(file_name);

    struct str_buf str_buf = str_buf_init(100);
    load_file(&str_buf, file_name);
    buffer_fill(buffer, str_buf);
    str_buf_deinit(&str_buf);

    buffer->dirty = 0;
}

void
fm_save()
{
    struct buffer* buffer = buffer_current();

    if (buffer->file_name == NULL)
    {
	return;
    }

    struct str_buf buffer_str = buffer_serialize(buffer);

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
fm_insert_row()
{
    struct buffer* buffer = buffer_current();

    if (buffer->cp.x == 0)
    {
        buffer_append_row(buffer, buffer->cp.y, cstr(""));
    }
    else
    {
	struct row *row = buffer->row + buffer->cp.y;
	buffer_append_row(buffer, buffer->cp.y + 1, cstrn(row->chars.data + buffer->cp.x, row->chars.size - buffer->cp.x));
	row = buffer->row + buffer->cp.y;
	row_resize(row, buffer->cp.x);
    }
    buffer->cp.y++;
    buffer->cp.x = 0;
}

void
fm_insert_char(char c)
{
    struct buffer* buffer = buffer_current();

    int index = buffer->cp.x;
    
    if (buffer->cp.y == buffer->num_rows)
    {
	buffer_append_row(buffer, buffer->num_rows, cstr(""));
    }
    
    struct row* row = buffer->row + buffer->cp.y;
    row_insert_char(row, index, c);
    buffer->cp.x++;
    buffer->dirty++;
}

void
fm_remove_char()
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
	buffer_delete_row(buffer, buffer->cp.y);
	buffer->cp.y--;
	buffer->dirty++;
    }
}

void
fm_exit()
{
    struct buffer* buffer = buffer_current();

    static int quit_times = FORME_QUIT_TIMES;

    if (buffer->dirty > 0 && quit_times > 0)
    {
	sm_set_message("File has been changed. "
		       "Please press Ctrl-Q %d to quit without saving", quit_times);
	quit_times--;
	return;
    }

    restore();
    buffer_deinit(*buffer);
    exit(0);

    quit_times = FORME_QUIT_TIMES;
}    
