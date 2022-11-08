#include "row.h"

static
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
    row->render_chars = str_buf_init(row->chars.size + tabs * (FORME_NUMBER_OF_SPACES_FOR_TAB - 1));

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

    row->render_chars.size = idx;
}

void
row_init(struct row* row)
{
    row->chars = str_buf_init(50);
    row->render_chars = str_buf_init(50);
}

void
row_deinit(struct row* row)
{
    str_buf_deinit(&row->chars);
    str_buf_deinit(&row->render_chars);
}

void
row_resize(struct row* row, int size)
{
    str_buf_resize(&row->chars, size);
    row_update(row);
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

    str_buf_remove_char(&row->chars, index);
    row_update(row);
}

void
row_insert_char(struct row* row, int index, char c)
{
    if (index < 0 || index > row->chars.size)
    {
	index = row->chars.size;
    }

    str_buf_insert_char(&row->chars, index, c);
    row_update(row);
}

void
row_append_string(struct row* row, struct str string)
{
    str_buf_append_str(&row->chars, string);
    row_update(row);
}
