#include "editor.h"

fint32
main(fint32 argc, fchar *argv[])
{
    editor_init();

    if (argc >= 2)
    {
	editor_open(argv[1]);
    }
    
    while(1)
    {
	editor_update();
    }

    return 0;
}
