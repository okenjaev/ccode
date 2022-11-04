#include "row.h"
#include <stdlib.h>

void
row_clean(struct row* row)
{
    free(row->data);
    free(row->render);
}

void
row_update(struct row* row)
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
    row->render = malloc(row->size + tabs * (FORME_NUMBER_OF_SPACES_FOR_TAB - 1) + 1);

    int idx = 0;
    for (int j = 0; j < row->size; j++)
    {
	if (row->data[j] == '\t')
	{
	    row->render[idx++] = ' ';
	    while(idx % FORME_NUMBER_OF_SPACES_FOR_TAB != 0)
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

int
row_cx_to_rx(const struct row* row, int cx)
{
    int rx = 0;
    for(int i =0; i < cx; i++)
    {
	if (row->data[i] == '\t')
	{
	    rx +=
		(FORME_NUMBER_OF_SPACES_FOR_TAB - 1) -
		(rx % FORME_NUMBER_OF_SPACES_FOR_TAB);
	}
	rx++;
    }
    return rx;
}

void
row_remove_char(struct row* row, int index)
{
    if (index < 0 || index >= row->size)
    {
	return;
    }

    memmove(row->data + index, row->data+index+1, row->size - index);
    row->size--;
    row_update(row);
}

void
row_insert_char(struct row* row, int index, char c)
{
    if (index < 0 || index > row->size)
    {
	index = row->size;
    }

    row->data = realloc(row->data, row->size + 2);
    memmove(row->data + index + 1, row->data + index, row->size - index + 1);
    row->size++;
    row->data[index] = c;
    row_update(row);
}

void
row_append_string(struct row* row, char* string, int len)
{
    row->data = realloc(row->data, row->size + len + 1);
    memcpy(row->data + row->size, string, len);
    row->size += len;
    row->data[row->size] = '\0';
    row_update(row);
}
