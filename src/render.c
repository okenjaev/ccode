#include "render.h"
#include "str.h"

int
render_flush(const struct str str)
{
    return write(STDOUT_FILENO, str.data, str.size);
}
