#include "renderb.h"

void
renderb_append(struct renderb *renderb, const char* s, int len)
{
    char *new = realloc(renderb->data, renderb->len + len);
    if (new == NULL)
    {
	return;
    }
    memcpy(&new[renderb->len], s, len);
    renderb->data = new;
    renderb->len += len;
}

void
renderb_free(struct renderb *renderb)
{
    free(renderb->data);
}
