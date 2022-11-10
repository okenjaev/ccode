#include "editor.h"
#include "sm.h"

int
main(int argc, char *argv[])
{
    editor_init();

    if (argc >= 2)
    {
	editor_open(argv[1]);
    }

    py_run();

    while(1)
    {
	editor_update();
    }

    return 0;
}
