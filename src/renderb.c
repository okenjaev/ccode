#include "renderb.h"

void
renderb_append(struct renderb *renderb, const char* s, int len)
{
    char *new = realloc(buffer->data, buffer->len + len);
    if (new == NULL)
    {
	return;
    }
    memcpy(&new[buffer->len], s, len);
    buffer->data = new;
    buffer->len += len;
}

void
renderb_free(struct renderb *renderb)
{
    free(buffer->data);
}
