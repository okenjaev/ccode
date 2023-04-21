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
    hotkeys = darray_init(int, 1);
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

    if (c == 'x')
    {
	sm_set_message("size:%u cap:%u es:%u i:%d", darray_size(hotkeys), *nc_darray_cap((char*) hotkeys), *nc_darray_es((char*) hotkeys), hotkeys[1]);
    }

    if (c == 'o')
    {
	darray_append(hotkeys, 7);
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
