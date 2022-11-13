#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"

void
setkbd(char* map, void (*func)(void))
{
    func();
}

void
input_update(void)
{    
    char c = read_key();

    if (c == FORME_CTRL_KEY('q'))
    {
        editor_exit();
    }
    else if (c == FORME_CTRL_KEY('o'))
    {
	buffer_open_file("txt");
    }
        
    sm_set_message("%c", c);
}

