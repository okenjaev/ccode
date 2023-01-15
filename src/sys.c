#include "sys.h"
#include "render.h"
#include "buffer.h"
#include "str.h"

struct config config;

static
void
get_cursor_position(fint32 *rows, fint32 *cols)
{
    fchar buf[32];
    fuint32 i = 0;

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
die(const fchar* s)
{
    restore();
    perror(s);
    exit(1);
}

void
restore()
{
    struct str_buf renderb = str_buf_init(10);
    str_buf_append(&renderb, cstrn("\x1b[2J", 4));
    str_buf_append(&renderb, cstrn("\x1b[H", 3));
    render_flush(renderb);
    str_buf_deinit(&renderb);    
}

void
get_window_size()
{
    struct winsize ws;

    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
	struct str_buf renderb = str_buf_init(12);
	str_buf_append(&renderb, cstrn("\x1b[999C\x1b[999B", 12));
	if (render_flush(renderb) != 12)
	{
	    str_buf_deinit(&renderb);
	    die("get_window_size");
	}
	else
	{
	    str_buf_deinit(&renderb);
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

// TODO: Maybe refactoring
void
load_file(struct str_buf* buf_str, const fchar* file_name)
{
    fint32 fp = open(file_name, O_RDONLY);
    if(fp == -1)
    {
	die("file_open");
    }

    fint32 size = lseek(fp, 0, SEEK_END);
    str_buf_resize(buf_str, size);
    lseek(fp, 0, SEEK_SET);
    
    if (size != read(fp, buf_str->data, buf_str->size))
    {
	die("faylni oqi ololmadim");
    }

    /* while ((linelen = getline(&line, &linecap, fp)) != -1) */
    /* {	 */
    /* 	while(linelen > 0 && (line[linelen - 1] == '\n' || */
    /* 			      line[linelen - 1] == '\r')) */
    /* 	{ */
    /* 	    linelen--; */
    /* 	} */
	
    /*     buffer_append_row(buffer, buffer->num_rows, line, linelen); */
    /* } */
    /* free(line); */

    close(fp);
}

fint32
write_to_file(const fchar* file_name, struct str_buf buffer_str)
{
    fint32 fd = open(file_name, O_RDWR | O_CREAT, 0644);
    if (fd != -1)
    {
	if (ftruncate(fd, buffer_str.size) != -1)
	{
	    if (write(fd, buffer_str.data, buffer_str.size) == buffer_str.size)
	    {
		close(fd);
		return 1;
	    }
	}
	close(fd);
    }
    return 0;
}

fchar
read_key()
{
    fchar c;

    fint32 nread;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
	if (nread == -1 && errno != EAGAIN)
	{
	    die("read");
	}
    }

    return c;
}
