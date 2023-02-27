#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"

#define FME_CTRL_KEY(k) ((k) & 0x1f)

static
void
input_empty_hk_buf(void);

static fchar last_key = 0;

static fchar* hotkeys[] =
{
    "ctrl-x ctrl-c",
    "ctrl-n",
    "ctrl-g",
    "ctrl-m", // enter
};

void (*actions[])(void) =
{
    editor_exit,
    buffer_cursor_next,
    input_empty_hk_buf,
    buffer_insert_row
};

fchar buf[100] = {0};

void
input_empty_hk_buf(void)
{
    memset(buf, 0, 100);
    sm_set_message("canceled!");
}

void
input_update(void)
{    
    fchar c = read_key();
    fint32 filled_buf = strlen(buf);
    
    if (c < 32) // last control-key is 31
    {
	if (c == FME_CTRL_KEY('q'))
	{
	    editor_exit();
	}

	fchar temp[10];
	sprintf(temp, "ctrl-%c", c + 96);
	strncat(buf, temp, 10);
    }
    else
    {
	if (filled_buf)
	{
	    strncat(buf, &c, 1);
	}
	else
	{
	    buffer_insert_fchar(c);
	}
    }

    fint32 found = 0;
    fint32 maxlen = 0;
    for (fint32 i = 0; i < 4; i++)
    {
	fchar* data = hotkeys[i];
	fint32 size = strlen(data);
	if (maxlen < size)
	{
	    maxlen = size;
	}
	if (strcmp(data, buf) == 0)
	{
	    found = 1;
	    actions[i]();
	    memset(buf, 0, 100);
	}
    }

    filled_buf = strlen(buf);
    if (found == 0 & maxlen < filled_buf)
    {
	memset(buf, 0, 100);
	sm_set_message("there's not hotkey");
	return;
    }

    if (filled_buf)
    {
	strcat(buf, " ");
	sm_set_message("%s", buf); 
    }
}

