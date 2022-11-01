#ifndef STR_H
#define STR_H

#include "common.h"

struct str {
    char* data;  
    int size;
};

#define STR_INIT {.size = 0, .data = NULL}

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
