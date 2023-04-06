#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"
#include "hk.h"

#define FME_CTRL_KEY(k) ((k) & 0x1f)
#define NUMBER_OF_HOT_KEYS 5
#define ASCII_CNTRL_OFFSET 96
#define ASCII_CNTRL_UPPER_LIMIT 32

// TODO: NEED TO BE REFACTORED

// If * is inserted in hotkey string
// then it should be evaluated dispate
// buffer is not empty

fchar buf[100];
fint32 size = 8;
static struct fhk* hotkeys;

static
void
input_empty_hk_buf(void)
{
    memset(buf, 0, 100);
    sm_set_message("canceled!");
}

void
input_init(void)
{
    hotkeys = malloc(sizeof(struct fhk) * size);
    hotkeys[0] = (struct fhk){"ctrl-x ctrl-c", editor_exit};
    hotkeys[1] = (struct fhk){"*ctrl-g", input_empty_hk_buf};
    hotkeys[2] = (struct fhk){"ctrl-d", buffer_remove_fchar};
    hotkeys[3] = (struct fhk){"ctrl-p", buffer_cursor_previous};
    hotkeys[4] = (struct fhk){"ctrl-n", buffer_cursor_next};
    hotkeys[5] = (struct fhk){"ctrl-f", buffer_cursor_forward};
    hotkeys[6] = (struct fhk){"ctrl-b", buffer_cursor_backward};
    hotkeys[7] = (struct fhk){"ctrl-m", buffer_insert_row};
}

void
input_update(void)
{    
    fchar c = read_key();
    fint32 filled_buf = strlen(buf);
    
    if (c < ASCII_CNTRL_UPPER_LIMIT)
    {
	// Temp in order to avoid program halt 
	fchar temp[10];
	sprintf(temp, "ctrl-%c", c + ASCII_CNTRL_OFFSET);

	for (fint32 i = 0; i < size; i++)
	{
	    struct fhk hk = hotkeys[i];
	    fchar* text = hk.text;
	    if (*text == '*')
	    {
		if (strcmp(text + 1, temp) == 0)
		{
		    hk.action();
		    return;
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
    for (fint32 i = 0; i < size; i++)
    {
	struct fhk data = hotkeys[i];
	fint32 size = strlen(data.text);
	if (maxlen < size)
	{
	    maxlen = size;
	}
	if (strcmp(data.text, buf) == 0)
	{
	    found = 1;
	    data.action();
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

void
input_deinit(void)
{
    free(hotkeys);
}
