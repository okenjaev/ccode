#include "editor.h"
#include "sys.h"
#include "render.h"
#include "input.h"
#include "str.h"
#include "row.h"
#include "sm.h"
#include "buffer.h"
#include "4me.h"

extern struct config config;

void
editor_init()
{
    enable_raw_mode();
    get_window_size();
    config.screenrows -= 2;
}

void
editor_open(const char* file_name)
{
    fm_buffer_open_file(file_name);
}

void
editor_update()
{
    struct buffer* buffer = buffer_current();
    buffer_update(buffer);
    render_draw(buffer);
    input_update(buffer);
}

