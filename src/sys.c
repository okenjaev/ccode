#include "sys.h"
#include "renderb.h"

struct config config;

static
void
get_cursor_position(int *rows, int *cols)
{
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    {
	die("get_cursor_position");
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
	die("get_cursor_position");
    }

    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
    {
	die("get_cursor_position");

    }    
}

void
die(const char* s)
{
    restore();
    perror(s);
    exit(1);
}

void
restore()
{
    struct renderb renderb = RENDERB_INIT;
    renderb_append(&renderb, "\x1b[2J", 4);
    renderb_append(&renderb, "\x1b[H", 3);
    renderb_flush(&renderb);
    renderb_free(&renderb);    
}


void
get_window_size()
{
    struct winsize ws;

    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
	struct renderb renderb = RENDERB_INIT;
	renderb_append(&renderb, "\x1b[999C\x1b[999B", 12);
	if (renderb_flush(&renderb) != 12)
	{
	    renderb_free(&renderb);
	    die("get_window_size");
	}
	else
	{
	    renderb_free(&renderb);
	}
	
        get_cursor_position(&config.screenrows, &config.screencols);
    }
    else
    {
	config.screencols = ws.ws_col;
        config.screenrows = ws.ws_row;
    }
}


void
disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &config.og_mode) == -1)
    {
        die("tcsetattr");
    }
}

void
enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &config.og_mode) == -1)
    {
        die("tsgetattr");
    }
    atexit(disable_raw_mode);
    
    struct termios raw = config.og_mode;
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
