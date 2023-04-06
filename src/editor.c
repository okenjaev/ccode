#include "editor.h"
#include "sys.h"
#include "render.h"
#include "input.h"
#include "str.h"
#include "row.h"
#include "sm.h"
#include "buffer.h"

extern struct config config;

void
editor_init()
{
    enable_raw_mode();
    get_window_size();
    config.screenrows -= 2;

    input_init();
}

void
editor_open(const fchar* file_name)
{
    buffer_open_file(file_name);
}

void
editor_update()
{
    struct buffer* buffer = buffer_current();
    buffer_update();
    render_draw(buffer);
    input_update();
}

void
editor_exit(void)
{
    struct buffer* buffer = buffer_current();
    restore();
    buffer_deinit(*buffer);

    input_deinit();
    exit(0);
}    
