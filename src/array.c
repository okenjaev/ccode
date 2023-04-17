#include "array.h"

struct darray_info
{
    fuint32 size;
    fuint32 capacity;
    fuint32 element_size;
};

fchar*
nc_darray_init(fuint32 elem_size, fuint32 cap) {		
    fchar* arr = malloc(sizeof(struct darray_info) +	
		 elem_size * cap);		
    arr += sizeof(struct darray_info);
    *(nc_darray_size(arr)) = 0;
    *(nc_darray_cap(arr)) = cap;
    *(nc_darray_es(arr)) = elem_size;
    return arr;
}

void
nc_darray_ensure_cap(char** arr)
{
    fuint32* size = nc_darray_size(*arr);
    fuint32* cap = nc_darray_cap(*arr);
    fuint32* es = nc_darray_es(*arr);

    if (*cap == *size - 1)
    {
	fuint32 desired_size = *cap * 2;
	
	fchar* temp = realloc(arr, sizeof(struct darray_info) +
			     *es * desired_size);
	if (temp)
	{
	    *arr = temp;
	    *cap = desired_size;
	}
    }
}

fuint32*
nc_darray_size(char* arr)
{
    fuint32 *size = (fuint32*)(arr - 3 * sizeof(fuint32));
    return size;
}

fuint32*
nc_darray_es(char* arr)
{
    fuint32 *es = (fuint32*)(arr - sizeof(fuint32));
    return es;
}

fuint32*
nc_darray_cap(char* arr)
{
    fuint32 *cp = (fuint32*)(arr - 2 * sizeof(fuint32));
    return cp;
}

void
nc_darray_free(char *arr)				
{							
    free(arr - sizeof(struct darray_info));	
}							



