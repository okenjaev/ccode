#include "buffer.h"

void
buffer_append(struct buffer *buffer, const char* s, int len)
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
buffer_free(struct buffer *buffer)
{
    free(buffer->data);
}
