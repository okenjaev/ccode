#ifndef STR_H
#define STR_H

#include "common.h"

struct str_buf
{
    fchar* data;
    fint32 size;
    fint32 capacity;
};  // mutable

#define str_buf_nan (struct str_buf){.data = NULL, .size = -1}
#define cstr(x) (struct str_buf){.data = x, .size = sizeof(x) - 1}
#define cstrn(x, y) (struct str_buf){.data = x, .size = y}

struct str_buf
str_buf_init(fint32);

void
str_buf_deinit(struct str_buf*);

void
str_buf_insert_fchar(struct str_buf* str_buf,
		    fint32 at_index,
		    fchar c);

void
str_buf_remove_fchar(struct str_buf* str_buf,
		    fint32 at_index);

void
str_buf_resize(struct str_buf* str_buf,
	       fint32 size);

void
str_buf_append(struct str_buf*,
	       const struct str_buf);

struct str_buf
str_buf_copy(const struct str_buf str);

fint32
str_buf_cmp(struct str_buf str, struct str_buf ano_str);

struct str_buf
str_buf_split(struct str_buf* str, const struct str_buf delim);


#endif /* STR_H */
