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
}

void
editor_open(const char* file_name)
{
    buffer_open_file(file_name);
}

void
editor_update()
{
    buffer_update();
    render_draw();
    input_update();
}

void
editor_exit(void)
{
    restore();
    buffer_deinit();
    exit(0);
}    
