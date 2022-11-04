#include "row.h"

void
row_clean(struct row* row)
{
    str_buf_deinit(&row->chars);
    str_buf_deinit(&row->render_chars);
}

void
row_update(struct row* row)
{
    int tabs = 0;

    for (int j =0; j < row->chars.size; j++)
    {
	if (row->chars.data[j] == '\t')
	{
	    tabs++;
	}
    }
    
    str_buf_deinit(&row->render_chars);
    row->render_chars = str_buf_init(row->chars.size + tabs * (FORME_NUMBER_OF_SPACES_FOR_TAB - 1) + 1);

    int idx = 0;
    for (int j = 0; j < row->chars.size; j++)
    {
	if (row->chars.data[j] == '\t')
	{
	    row->render_chars.data[idx++] = ' ';
	    while(idx % FORME_NUMBER_OF_SPACES_FOR_TAB != 0)
	    {
		row->render_chars.data[idx++] = ' ';
	    }
	}
	else
	{
	    row->render_chars.data[idx++] = row->chars.data[j];
	}
    }

    row->render_chars.data[idx] = '\0';
    row->render_chars.size = idx;
}

int
row_cx_to_rx(const struct row* row, int cx)
{
    int rx = 0;
    for(int i = 0; i < cx; i++)
    {
	if (row->chars.data[i] == '\t')
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
    if (index < 0 || index >= row->chars.size)
    {
	return;
    }

    memmove(row->chars.data + index, row->chars.data+index+1, row->chars.size - index);
    row->chars.size--;
    row_update(row);
}

void
row_insert_char(struct row* row, int index, char c)
{
    if (index < 0 || index > row->chars.size)
    {
	index = row->chars.size;
    }

    row->chars.data = realloc(row->chars.data, row->chars.size + 2);
    memmove(row->chars.data + index + 1, row->chars.data + index, row->chars.size - index + 1);
    row->chars.size++;
    row->chars.data[index] = c;
    row_update(row);
}

void
row_append_string(struct row* row, char* string, int len)
{
    row->chars.data = realloc(row->chars.data, row->chars.size + len + 1);
    memcpy(row->chars.data + row->chars.size, string, len);
    row->chars.size += len;
    row->chars.data[row->chars.size] = '\0';
    row_update(row);
}
