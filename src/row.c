#include "row.h"

static
void
row_update(struct row* row)
{
    fint32 tabs = 0;

    for (fint32 j =0; j < row->fchars.size; j++)
    {
	if (row->fchars.data[j] == '\t')
	{
	    tabs++;
	}
    }
    
    str_buf_deinit(&row->render_fchars);
    row->render_fchars = str_buf_init(row->fchars.size + tabs * (FORME_NUMBER_OF_SPACES_FOR_TAB - 1));

    fint32 idx = 0;
    for (fint32 j = 0; j < row->fchars.size; j++)
    {
	if (row->fchars.data[j] == '\t')
	{
	    row->render_fchars.data[idx++] = ' ';
	    while(idx % FORME_NUMBER_OF_SPACES_FOR_TAB != 0)
	    {
		row->render_fchars.data[idx++] = ' ';
	    }
	}
	else
	{
	    row->render_fchars.data[idx++] = row->fchars.data[j];
	}
    }

    row->render_fchars.size = idx;
}

void
row_init(struct row* row)
{
    row->fchars = str_buf_init(50);
    row->render_fchars = str_buf_init(50);
}

void
row_deinit(struct row* row)
{
    str_buf_deinit(&row->fchars);
    str_buf_deinit(&row->render_fchars);
}

void
row_resize(struct row* row, fint32 size)
{
    str_buf_resize(&row->fchars, size);
    row_update(row);
}

fint32
row_cx_to_rx(const struct row* row, fint32 cx)
{
    fint32 rx = 0;
    for(fint32 i = 0; i < cx; i++)
    {
	if (row->fchars.data[i] == '\t')
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
row_remove_fchar(struct row* row, fint32 index)
{
    if (index < 0 || index >= row->fchars.size)
    {
	return;
    }

    str_buf_remove_fchar(&row->fchars, index);
    row_update(row);
}

void
row_insert_fchar(struct row* row, fint32 index, fchar c)
{
    if (index < 0 || index > row->fchars.size)
    {
	index = row->fchars.size;
    }

    str_buf_insert_fchar(&row->fchars, index, c);
    row_update(row);
}

void
row_append_string(struct row* row, struct str_buf string)
{
    str_buf_append(&row->fchars, string);
    row_update(row);
}
