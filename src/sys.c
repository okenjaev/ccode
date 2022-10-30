#include "sys.h"
#include "common.h"
#include "buffer.h"

extern struct buffer e;

void
die(const char* s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    
    perror(s);
    exit(1);
}

int
get_window_size(int *rows, int *cols)
{
    struct winsize ws;

    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
	if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
	{
	    return -1;
	}
	return get_cursor_position(rows, cols);
    }
    else
    {
	*cols = ws.ws_col;
	*rows = ws.ws_row;
	return 0;
    }
}

int
get_cursor_position(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    {
	return -1;
    }

    while(i < sizeof(buf) - 1)
    {
	if (read(STDIN_FILENO, &buf[i], 1) != 1)
	{
	    break;
	}

	if (buf[i] == 'R')
	{
	    break;
	}

	i++;
    }

    buf[i] = '\0';

    if (buf[0] != '\x1b' || buf[1] != '[')
    {
	return -1;
    }

    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    {
	return -1;
    }    
    return 0;
}

void
disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &e.og_mode) == -1)
    {
        die("tcsetattr");
    }
}

void
enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &e.og_mode) == -1)
    {
        die("tsgetattr");
    }
    atexit(disable_raw_mode);
    
    struct termios raw = e.og_mode;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}
