#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "sys.h"
#include "editor.h"
#include "interp.h"
#include "array.h"

#define in_ctrl(x) (x & 0x1F)

static darray(int) hotkeys;

void
input_init(void)
{
    hotkeys = darray_init(int, 5);
}

void
input_add_hotkey(fchar* key, void* call_back)
{
    darray_append(hotkeys, 0);
    darray_append(hotkeys, 1);
    darray_append(hotkeys, 2);
    darray_append(hotkeys, 3);
    darray_append(hotkeys, 4);
    darray_append(hotkeys, 5);
    darray_append(hotkeys, 6);
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

    if (c == 'x')
    {
	sm_set_message("%u %d", darray_size(hotkeys), hotkeys[8]);
    }
}

void
input_deinit(void)
{
    /* for (int i=0; i < darray_size(hotkeys); i++) */
    /* { */
    /* 	struct hotkey hk = hotkeys[i]; */
    /* 	interp_release((void*)hk.func); */
    /* } */
    
    darray_free(hotkeys);
    hotkeys = NULL;
}
