#include "common.h"
#include "editor.h"

int main(int argc, char *argv[])
{
    term_enable_raw_mode();
    editor_init();
    
    while(1)
    {
	editor_refresh_screen();
	editor_process_keys();
    }

    return 0;
}
