#ifndef STR_H
#define STR_H

#include "common.h"

struct str {
    char* data;  
    int size;
};  // immutable

struct str_buf
{
    char* data;
    int size;
    int capacity;
};  // mutable

#define STR_INIT {.size = 0, .data = NULL}

#define cstr(x) (const struct str){.data = x, .size = sizeof(x) - 1}
#define cstrn(x, y) (const struct str){.data = x, .size = y}

/* str buf */
struct str
str_buf_str(const struct str_buf);

struct str_buf
str_buf_init(int);

void
str_buf_deinit(struct str_buf*);

void
str_buf_insert_char(struct str_buf* str_buf,
		    int at_index,
		    char c);

void
str_buf_remove_char(struct str_buf* str_buf,
		    int at_index);

void
str_buf_resize(struct str_buf* str_buf,
	       int size);

void
str_buf_append(struct str_buf*,
	       const struct str);

// TODO: temporary need to be removed
void
str_buf_append_raw(struct str_buf*,
		   char* string,
		   int len);

/* str */

struct str
str_init_format(const char* fmt, ...);

struct str
str_concat(struct str str, const struct str ano_str);

void
str_append(struct str* str, const struct str ano_str);

void
str_append_raw(struct str* str, const char* s, int len);

struct str
str_copy(const struct str str);

void
str_deinit(struct str* str);

#endif /* STR_H */
