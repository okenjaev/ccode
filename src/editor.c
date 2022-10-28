#include "editor.h"

extern struct term_config e;

enum editor_keys
{
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME_KEY,
    END_KEY,
    DEL_KEY
};

void
editor_move_cursor(int key)
{
    switch (key)
    {
    case ARROW_LEFT:
	if (e.cx != 0)
	{
	    e.cx--;	    
	}
	break;
    case ARROW_RIGHT:
	if (e.cy != e.screencols - 1)
	{
	    e.cx++;	    
	}
	break;
    case ARROW_UP:
	if (e.cy != 0)
	{
	    e.cy--;	    
	}
	break;
    case ARROW_DOWN:
	if (e.cy != e.screenrows - 1)
	{
	    e.cy++;	    
	}
	break;
    }
}

void
editor_draw_rows(struct buffer* buffer)
{
    for(int y=0; y<e.screenrows; y++)
    {
	if (y == e.screenrows / 3)
	{
	    char welcome[80];
	    int welcomelen = snprintf(welcome, sizeof(welcome),
				      "4me version --version %s", FORME_VERSION);
	    if (welcomelen > e.screencols)
	    {
		welcomelen = e.screencols;
	    }

	    int padding = (e.screencols - welcomelen) / 2;
	    if (padding)
	    {
	        buffer_append(buffer, "~", 1);
		padding--;
	    }
	    while (padding--)
	    {
		buffer_append(buffer, " ", 1);
	    }
	    buffer_append(buffer, welcome, welcomelen);
	} else {
	    buffer_append(buffer, "~", 1);	    
	}

	buffer_append(buffer, "\x1b[K", 3);
	if (y < e.screenrows - 1) {
	    buffer_append(buffer, "\r\n", 2);
	}
    }
}

void
editor_init()
{
    e.cx = 0;
    e.cy = 0;
    if (term_get_window_size(&e.screenrows, &e.screencols) == -1)
    {
	term_die("getwindowsize");
    }
}

void
editor_refresh_screen()
{
    struct buffer buffer = BUFFER_INIT;

    buffer_append(&buffer, "\x1b[?25l", 6);
    buffer_append(&buffer, "\x1b[H", 3);

    editor_draw_rows(&buffer);

    char cur_pos[32];
    snprintf(cur_pos, sizeof(cur_pos), "\x1b[%d;%dH", e.cy + 1, e.cx + 1);
    buffer_append(&buffer, cur_pos, strlen(cur_pos));

    buffer_append(&buffer, "\x1b[?25h", 6);
    write(STDOUT_FILENO, buffer.data, buffer.len);
    buffer_free(&buffer);
}

int
editor_read_key()
{
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
	if (nread == -1 && errno != EAGAIN)
	{
	    term_die("read");
	}
    }

    if (c == '\x1b')
    {
	char seq[3];

	if (read(STDIN_FILENO, &seq[0], 1) != 1)
	{
	    return '\x1b';
	}

	if (read(STDIN_FILENO, &seq[1], 1) != 1)
	{
	    return '\x1b';
	}

	if (seq[0] == '[')
	{
	    if (seq[1] >= '0' && seq[1] <= '9')
	    {
		if (read(STDIN_FILENO, &seq[2], 1) != 1)
		{
		    return '\x1b';
		}

		if (seq[2] == '~')
		{
		    switch(seq[1])
		    {
		    case '1':
			return HOME_KEY;
		    case '3':
			return DEL_KEY;
		    case '4':
			return END_KEY;
		    case '5':
			return PAGE_UP;
		    case '6':
			return PAGE_DOWN;
		    case '7':
			return HOME_KEY;
		    case '8':
			return END_KEY;
		    }
		}
	    }
	    else
	    {
		switch(seq[1])
		{
		case 'A':
		    return ARROW_UP;
		case 'B':
		    return ARROW_DOWN;
		case 'C':
		    return ARROW_RIGHT;
		case 'D':
		    return ARROW_LEFT;
		case 'H':
		    return HOME_KEY;
		case 'F':
		    return END_KEY;
		}
	    }
	}
	else if (seq[0] == 'O')
	{
	    switch(seq[1])
	    {
	    case 'H':
		return HOME_KEY;
	    case 'F':
		return END_KEY;
	    }
	}

	return '\x1b';
    }
    else
    {
	return c;	
    }
    
}

void
editor_process_keys()
{
    int c = editor_read_key();
    switch(c)
    {
    case CTRL_KEY('q'):
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	exit(0);
	break;
    case ARROW_UP:
    case ARROW_LEFT:
    case ARROW_RIGHT:
    case ARROW_DOWN:
	editor_move_cursor(c);
	break;

    case HOME_KEY:
	e.cx = 0;
	break;
    case END_KEY:
	e.cx = e.screencols -1;
	break;
	
    case PAGE_UP:
    case PAGE_DOWN:
    {
	int times = e.screenrows;
	while(times--)
	{
	    editor_move_cursor(c == PAGE_UP? ARROW_UP: ARROW_DOWN);
	}
    }
    break;
    }
}
