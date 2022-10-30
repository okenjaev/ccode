#include "common.h"
#include "editor.h"

int main(int argc, char *argv[])
{
    term_enable_raw_mode();
    editor_init();

    if (argc >= 2)
    {
	editor_open(argv[1]);
    }

    editor_set_status_message("HELP: Ctrl-q to quit");
    while(1)
    {
	editor_refresh_screen();
	editor_process_keys();
    }

    return 0;
}
