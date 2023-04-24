#ifndef ARRAY_H
#define ARRAY_H

#include "common.h"

#define darray(x) x*
#define darray_init(es, cp) (es*)nc_darray_init(sizeof(es), cp)
#define darray_free(x) nc_darray_free((char*)x)
#define darray_size(x) *nc_darray_size((char*)x)
#define darray_remove(x, y) nc_darray_remove((char*)x, y)

#define darray_append(x, y)				\
    {							\
        nc_darray_ensure_cap((char **) &x);		\
	fuint32 *size = nc_darray_size((char*) x);	\
	x[*size] = y;					\
	*size += 1;					\
    }							\

void
nc_darray_ensure_cap(char** arr);

void
nc_darray_remove(char* arr, fuint32 index);

fchar*
nc_darray_init(fuint32 elem_size, fuint32 cap);    

fuint32*
nc_darray_size(char* arr);

fuint32*
nc_darray_cap(char* arr);

fuint32*
nc_darray_es(char* arr);

void
nc_darray_free(char *arr);

#endif /* ARRAY_H */
