#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"

#define FME_CTRL_KEY(k) ((k) & 0x1f)
#define NUMBER_OF_HOT_KEYS 4
#define ASCII_CNTRL_OFFSET 96
#define ASCII_CNTRL_UPPER_LIMIT 32

// TODO: NEED TO BE REFACTORED

static
void
input_empty_hk_buf(void);

static fchar last_key = 0;


// If * is inserted in hotkey string
// then it should be evaluated dispate
// buffer is not empty

static fchar* hotkeys[] =
{
    "ctrl-x ctrl-c", // exit
    "ctrl-n", // next line
    "*ctrl-g", // discard hotkeys
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
    
    if (c < ASCII_CNTRL_UPPER_LIMIT) // last control-key is 31
    {
	// Temp in order to avoid program halt 
	if (c == FME_CTRL_KEY('q'))
	{
	    editor_exit();
	}

	fchar temp[10];
	sprintf(temp, "ctrl-%c", c + ASCII_CNTRL_OFFSET);

	for (fint32 i = 0; i < NUMBER_OF_HOT_KEYS; i++)
	{
	    fchar* hk = hotkeys[i];
	    if (*hk == '*')
	    {
		if (strcmp((hk + 1), temp) == 0)
		{
		    actions[i]();
		}
	    }
	}

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
    for (fint32 i = 0; i < NUMBER_OF_HOT_KEYS; i++)
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

