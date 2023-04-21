#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "sys.h"
#include "editor.h"
#include "interp.h"
#include "array.h"

#define in_ctrl(x) (x & 0x1F)

void
input_init(void)
{

}

void
input_add_hotkey(fchar* key, void* call_back)
{
    sm_set_message("%s", "python loaded");
}

void
input_update(void)
{    
    fchar c = read_key();

    if (c < 32)
    {
	/* detect_key(); */
	return;
    }
    
    buffer_insert_fchar(c);

    if (c == 'q')
    {
	editor_exit();
    }

}

void
input_deinit(void)
{

}
