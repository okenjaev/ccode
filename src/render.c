#include "render.h"

void
render(const struct buffer* buffer)
{
    write(STDOUT_FILENO, buffer->data, buffer->len);
}
