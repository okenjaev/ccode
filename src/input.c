#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "4me.h"
#include "py_int.h"
#include "sys.h"

/* static */
/* void */
/* print_num(int n, char* s) */
/* { */
/*     char* it = s; */
    
/*     while (it - s < 8) { */
/* 	if (n & 1) */
/* 	    *it = '1'; */
/* 	else */
/* 	    *it = '0'; */

/* 	n >>= 1; */
/* 	it++; */
/*     } */
/* } */

void
input_update()
{    
    char c = read_key();

    if (iscntrl(c))
    {
	if (c == FORME_CTRL_KEY('q'))
	{
	    fm_exit();
	}
	else if (c == FORME_CTRL_KEY('p'))
	{
	    fm_previous();
	}
	else if (c == FORME_CTRL_KEY('n'))
	{
	    fm_next();
	}
	else if (c == FORME_CTRL_KEY('b'))
	{
	    fm_backward();
	}
	else if (c == FORME_CTRL_KEY('f'))
	{
	    fm_forward();
	}
	else if (c == FORME_CTRL_KEY('d'))
	{
	    // TODO: bug with removing new line
	    fm_forward();
	    fm_remove_char();
	}
    }
    else
    {
	fm_insert_char(c);
    }

    sm_set_message("%c", c);
}

