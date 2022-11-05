#include "editor.h"
#include "sys.h"
#include "render.h"
#include "input.h"
#include "str.h"
#include "row.h"

extern struct config config;

void
editor_init()
{
    enable_raw_mode();
    get_window_size();
    config.screenrows -= 2;
}

struct buffer current_buffer = BUFFER_INIT;

void
editor_set_status_message(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(current_buffer.status_message, sizeof(current_buffer.status_message), fmt, ap);
    va_end(ap);
    current_buffer.status_message_time = time(NULL);
}

void
editor_save(struct buffer* buffer)
{
    if (buffer->file_name == NULL)
    {
	return;
    }

    struct str_buf buffer_str = buffer_serialize(buffer);

    int fd = open(buffer->file_name, O_RDWR | O_CREAT, 0644);
    if (fd != -1)
    {
	if (ftruncate(fd, buffer_str.size) != -1)
	{
	    if (write(fd, buffer_str.data, buffer_str.size) == buffer_str.size)
	    {
		close(fd);
		editor_set_status_message("%d bytes has been saved to disk", buffer_str.size);
		str_buf_deinit(&buffer_str);	
		buffer->dirty = 0;
		return;
	    }
	}
	close(fd);
    }

    str_buf_deinit(&buffer_str);
    editor_set_status_message("Error: Can't save file %s", strerror(errno));
}

void
editor_open(const char* file_name)
{
    free(current_buffer.file_name);
    current_buffer.file_name = strdup(file_name);
    
    load_file(&current_buffer, file_name);
    current_buffer.dirty = 0;
}

void
editor_draw_update()
{
    buffer_cursor_update(&current_buffer);
    
    struct str renderb = STR_INIT;

    str_append_raw(&renderb, "\x1b[?25l", 6);
    str_append_raw(&renderb, "\x1b[H", 3);

    render_draw_rows(&current_buffer, &renderb);
    render_draw_status_bar(&current_buffer, &renderb);
    render_draw_status_message(&current_buffer, &renderb);
    render_set_cursor_position(&current_buffer, &renderb);
    
    str_append_raw(&renderb, "\x1b[?25h", 6);

    render_flush(renderb);
    str_deinit(&renderb);
}

void
editor_input_update()
{
    static int quit_times = FORME_QUIT_TIMES;
    
    int c = editor_read_key();
    switch(c)
    {
    case CTRL_KEY('o'):
	editor_open("../txt");
	return;
	
    case CTRL_KEY('l'):
    case '\x1b':
	break;

    case CTRL_KEY('h'):
    case BACKSPACE:
    case DEL_KEY:
	if (c == DEL_KEY)
	{
	    buffer_move_cursor(&current_buffer, ARROW_RIGHT);
	}
	buffer_remove_char(&current_buffer);
	break;

    case '\r':
	buffer_insert_row(&current_buffer);
	break;

    case CTRL_KEY('s'):
	editor_save(&current_buffer);
	break;

    case CTRL_KEY('q'):
	if (current_buffer.dirty > 0 && quit_times > 0)
	{
	    editor_set_status_message("File has been changed. "
				      "Please press Ctrl-Q %d to quit without saving", quit_times);
	    quit_times--;
	    return;
	}

	restore();
	exit(0);
	break;
	
    case ARROW_UP:
    case ARROW_LEFT:
    case ARROW_RIGHT:
    case ARROW_DOWN:
	buffer_move_cursor(&current_buffer, c);
	break;
    break;
    default:
        buffer_insert_char(&current_buffer, current_buffer.cp.x, c);
    }

    quit_times = FORME_QUIT_TIMES;
}
