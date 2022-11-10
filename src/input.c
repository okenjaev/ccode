#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "4me.h"
#include "py_int.h"

static
int
editor_read_key()
{
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
	if (nread == -1 && errno != EAGAIN)
	{
	    die("read");
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
input_update()
{    
    int c = editor_read_key();
    switch(c)
    {
    case CTRL_KEY('o'):
	/* exit(0); */
	fm_open_file("../txt");
	return;
	
    case CTRL_KEY('l'):
    case '\x1b':
	break;

    case CTRL_KEY('h'):
    case BACKSPACE:
    case DEL_KEY:
	if (c == DEL_KEY)
	{
	    fm_forward();
	}
	fm_remove_char();
	break;

    case '\r':
	fm_insert_row();
	break;

    case CTRL_KEY('s'):
	fm_save();
	break;

    case CTRL_KEY('q'):
	fm_exit();
	break;
	
    case ARROW_UP:
	fm_previous();
	break;
    case ARROW_LEFT:
	fm_backward();
	break;
    case ARROW_RIGHT:
	fm_forward();
	break;
    case ARROW_DOWN:
	fm_next();
	break;
    break;
    default:
        fm_insert_char(c);
    }

}
