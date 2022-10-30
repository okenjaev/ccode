#include "render.h"

void
render(const struct renderb* renderb)
{
    write(STDOUT_FILENO, renderb->data, renderb->len);
}
