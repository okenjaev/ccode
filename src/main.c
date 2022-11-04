#include "editor.h"

int
main(int argc, char *argv[])
{
    editor_init();

    if (argc >= 2)
    {
	editor_open(argv[1]);
    }

    editor_set_status_message("HELP: Ctrl-q to quit");

    while(1)
    {
	editor_draw_update();
	editor_input_update();
    }

    return 0;
}
