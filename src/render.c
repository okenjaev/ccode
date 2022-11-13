#include "render.h"
#include "str.h"
#include "buffer.h"
#include "sys.h"
#include "row.h"
#include "sm.h"

extern struct config config;
extern struct sm sm;

void
render_set_cursor_position(struct str_buf* renderb)
{
    const struct cur_pos cp = buffer_cur_pos();
    int y = cp.y - cp.rowoff + 1;
    int x = cp.r - cp.coloff + 1;
    char curpos[16];
    snprintf(curpos, sizeof(curpos), "\x1b[%d;%dH", y, x);
    str_buf_append(renderb, cstrn(curpos, strlen(curpos)));
}

void
render_draw_status_bar(struct str_buf* renderb)
{
    str_buf_append(renderb, cstrn("\x1b[7m", 4));

    const char* file_name = buffer_file_name();
    const struct cur_pos cp = buffer_cur_pos();
    const int dirty = buffer_dirty();
    
    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), "%.20s %s",
		       file_name ? file_name : "[empty]", dirty ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d", cp.y + 1);
    
    if (len > config.screencols)
    {
	len = config.screencols;
    }

    str_buf_append(renderb, cstrn(status, len));
    while(len < config.screencols)
    {
	if (config.screencols - len == rlen)
	{
	    str_buf_append(renderb, cstrn(rstatus, rlen));
	    break;
	}
	else
	{
	    str_buf_insert_char(renderb, renderb->size, ' ');
	    len++;
	}
    }

    str_buf_append(renderb, cstrn("\x1b[m", 3));
    str_buf_append(renderb, cstrn("\r\n", 2));
}

void
render_draw_status_message(struct str_buf* renderb)
{
    str_buf_append(renderb, cstrn("\x1b[K", 3));
    int meslen = strlen(sm.message);

    if (meslen > config.screencols){
	meslen = config.screencols;
    }

    if (meslen && time(NULL) - sm.time < 5)
    {
	str_buf_append(renderb, cstrn(sm.message, meslen));	
    }
}

struct str_buf renderb = {.data = NULL, .size = 0, .capacity = 0};

int
render_flush(const struct str_buf str)
{
    return write(STDOUT_FILENO, str.data, str.size);
}

void render_draw(void)
{
    str_buf_resize(&renderb, 0);
    str_buf_append(&renderb, cstrn("\x1b[?25l", 6));
    str_buf_append(&renderb, cstrn("\x1b[H", 3));

    buffer_convert_to_render(&renderb);
    render_draw_status_bar(&renderb);
    render_draw_status_message(&renderb);
    render_set_cursor_position(&renderb);

    str_buf_append(&renderb, cstrn("\x1b[?25h", 6));
    render_flush(renderb);
}

