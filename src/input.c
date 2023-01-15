#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "py_int.h"
#include "sys.h"
#include "editor.h"

void
input_update(void)
{    
    fchar c = read_key();

    if (c == FORME_CTRL_KEY('q')) 
    {
        editor_exit();
    }


    
    sm_set_message("%c", c);
}

