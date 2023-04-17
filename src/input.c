#include "input.h"
#include "sys.h"
#include "buffer.h"
#include "sm.h"
#include "sys.h"
#include "editor.h"
#include "interp.h"
#include "array.h"

#define in_ctrl(x) (x & 0x1F)

struct hotkey
{
    char* text;
    void* func;
};

static darray(struct hotkey) hotkeys;

void
input_init(void)
{
    hotkeys = darray_init(struct hotkey, 10);
}

void
input_add_hotkey(fchar* key, void* call_back)
{
    
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
    for (int i=0; i < darray_size(hotkeys); i++)
    {
	struct hotkey hk = hotkeys[i];
	interp_release((void*)hk.func);
    }
    
    darray_free(hotkeys);
    hotkeys = NULL;
}
