#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

#define BAZAAR_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)

struct abuf
{
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0}

void
ab_append(struct abuf *ab, const char* s, int len)
{
    char *new = realloc(ab->b, ab->len + len);
    if (new == NULL)
    {
	return;
    }
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void ab_free(struct abuf *ab)
{
    free(ab->b);
}
    
char editor_read_key();

struct editor_config
{
    int screenrows;
    int screencols;
    struct termios og_mode;    
};

struct editor_config e;

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
    int get_cursor_position(int* rows, int* cols);
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

    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
    
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
    };
}

void
editor_draw_rows(struct abuf* ab)
{
    for(int y=0; y<e.screenrows; y++)
    {
	if (y == e.screenrows / 3)
	{
	    char welcome[80];
	    int welcomelen = snprintf(welcome, sizeof(welcome),
				      "BAZAAR version --version %s", BAZAAR_VERSION);
	    if (welcomelen > e.screencols)
	    {
		welcomelen = e.screencols;
	    }

	    int padding = (e.screencols - welcomelen) / 2;
	    if (padding)
	    {
		ab_append(ab, "~", 1);
		padding--;
	    }
	    while (padding--) ab_append(ab, " ", 1);
	    ab_append(ab, welcome, welcomelen);
	} else {
	    ab_append(ab, "~", 1);	    
	}

	ab_append(ab, "\x1b[K", 3);
	if (y < e.screenrows - 1) {
	    ab_append(ab, "\r\n", 2);
	}
    }
}

void
editor_init()
{
    if (get_window_size(&e.screenrows, &e.screencols) == -1)
    {
	die("getwindowsize");
    }
}

void
editor_refresh_screen()
{
    struct abuf ab = ABUF_INIT;

    ab_append(&ab, "\x1b[?25l", 6);
    ab_append(&ab, "\x1b[H", 3);

    editor_draw_rows(&ab);

    ab_append(&ab, "\x1b[H", 3);    
    ab_append(&ab, "\x1b[?25h", 6);
    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}

char
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
    return c;
}

void
editor_process_keys()
{
    char c = editor_read_key();
    switch(c)
    {
    case CTRL_KEY('q'):
	exit(0);
	break;
    }
}

int main(int argc, char *argv[])
{
    enable_raw_mode();
    editor_init();
    
    while(1)
    {
	editor_refresh_screen();
	editor_process_keys();
    }
    return 0;
}
