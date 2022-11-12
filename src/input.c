#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "4me.h"
#include "py_int.h"
#include "sys.h"

void setkbd(char* map, void (*func)(void))
{
    func();
}

void
input_update(void)
{    
    char c = read_key();

    if (c == FORME_CTRL_KEY('q'))
    {
	fm_exit();
    }

    sm_set_message("%c", c);
}

