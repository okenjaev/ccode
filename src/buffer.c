#include "buffer.h"
#include "sys.h"

extern struct config config;

static
int
buffer_row_cx_to_rx(const struct buffer_row* row, int cx)
{
    int rx = 0;
    for(int i =0; i < cx; i++)
    {
	if (row->data[i] == '\t')
	{
	    rx +=
		(NUMBER_OF_SPACES_FOR_TAB - 1) -
		(rx % NUMBER_OF_SPACES_FOR_TAB);
	}
	rx++;
    }
    return rx;
}

static
void
buffer_row_update(struct buffer_row* row)
{
    int tabs = 0;

    for (int j =0; j < row->size; j++)
    {
	if (row->data[j] == '\t')
	{
	    tabs++;
	}
    }
    
    free(row->render);
    row->render = malloc(row->size + tabs * (NUMBER_OF_SPACES_FOR_TAB - 1) + 1);

    int idx = 0;
    for (int j = 0; j < row->size; j++)
    {
	if (row->data[j] == '\t')
	{
	    row->render[idx++] = ' ';
	    while(idx % NUMBER_OF_SPACES_FOR_TAB != 0)
	    {
		row->render[idx++] = ' ';
	    }
	}
	else
	{
	    row->render[idx++] = row->data[j];
	}
    }

    row->render[idx] = '\0';
    row->rsize = idx;
}

void
buffer_scroll_update(struct buffer* buffer)
{
    buffer->cp.r = buffer->cp.x;

    if (buffer->cp.y < buffer->num_rows)
    {
	buffer->cp.r = buffer_row_cx_to_rx(&buffer->row[buffer->cp.y], buffer->cp.x);
    }

    if (buffer->cp.y < buffer->cp.rowoff)
    {
	buffer->cp.rowoff = buffer->cp.y;
    }

    if (buffer->cp.y >= buffer->cp.rowoff + config.screenrows)
    {
	buffer->cp.rowoff = buffer->cp.y - config.screenrows + 1;
    }

    if (buffer->cp.r < buffer->cp.coloff) {
	buffer->cp.coloff = buffer->cp.r;
    }
    
    if (buffer->cp.r >= buffer->cp.coloff + config.screencols) {
	buffer->cp.coloff = buffer->cp.r - config.screencols + 1;
    }
}


void
buffer_append_row(struct buffer* buffer, char* string, int len)
{
    buffer->row = realloc(buffer->row, sizeof(struct buffer_row) * (buffer->num_rows + 1));

    struct buffer_row *at = &buffer->row[buffer->num_rows];
    
    at->size = len;
    at->data = malloc(len + 1);
    memcpy(at->data, string, len);
    at->data[len] = '\0';
    at->rsize = 0;
    at->render = NULL;

    buffer_row_update(at);
    
    buffer->num_rows++;
}

