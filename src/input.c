#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "sys.h"
#include "editor.h"
#include "hotkey.h"
#include "interp.h"

static void *my_callback;

void
input_init(void)
{
    
}

void
input_add_hotkey(void* call_back)
{
    my_callback = call_back;
    sm_set_message("Loaded");
}

void
input_update(void)
{    
    fchar c = read_key();
    if (c == 'q')
    {
	editor_exit();
    }
    interp_call(my_callback);
}

void
input_deinit(void)
{

}
