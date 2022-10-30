#include "buffer.h"

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
buffer_init(struct buffer* buffer)
{
    buffer->cx = 0;
    buffer->cy = 0;
    buffer->rx = 0;
    buffer->num_rows = 0;
    buffer->row = NULL;
    buffer->rowoff = 0;
    buffer->coloff = 0;
    buffer->file_name = NULL;
    buffer->status_message[0] = '\0';
    buffer->status_message_time = 0;
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

