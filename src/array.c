#include "array.h"
#include "sm.h"

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

    if (*cap == *size)
    {
	fuint32 desired_size = *cap * 2;
	fuint32* es = nc_darray_es(*arr);
	
	fchar* temp = realloc(*arr - sizeof(struct darray_info),
			      sizeof(struct darray_info) + *es * desired_size);
	if (temp)
	{

	    temp += sizeof(struct darray_info);
	    *arr = temp;
	    *(nc_darray_cap(*arr)) = desired_size;

	    /* sm_set_message("%u", *cap); */
	}
    }
}

fuint32*
nc_darray_size(char* arr)
{
    return (fuint32*)(arr - 3 * sizeof(fuint32));
}

fuint32*
nc_darray_cap(char* arr)
{
    return (fuint32*)(arr - 2 * sizeof(fuint32));
}

fuint32*
nc_darray_es(char* arr)
{
    return (fuint32*)(arr - sizeof(fuint32));
}

void
nc_darray_free(char *arr)				
{							
    free(arr - sizeof(struct darray_info));	
}							



