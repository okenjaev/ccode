#include "str.h"

struct str
str_init_format(const char* fmt, ...)
{
    struct str res;
    res.size = 80;
    res.data = malloc(sizeof(char) * res.size);
    
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(res.data, res.size, fmt, ap);
    va_end(ap);
    return res;
}

void
str_append(struct str* str, const struct str ano_str)
{
    char *new = realloc(str->data, str->size + ano_str.size);
    if (new == NULL)
    {
	return;
    }
    memcpy(new + str->size, ano_str.data, ano_str.size);
    str->data = new;
    str->size += ano_str.size;
}

void
str_append_raw(struct str* str, const char* s, int len)
{
    char *new = realloc(str->data, str->size + len);
    if (new == NULL)
    {
	return;
    }
    memcpy(new + str->size, s, len);
    str->data = new;
    str->size += len;
}

struct str
str_concat(struct str str, const struct str ano_str)
{
    struct str res;
    res.size = str.size + ano_str.size;
    res.data = malloc(sizeof(char) * res.size);
    memcpy(res.data, str.data, str.size);
    memcpy(res.data + str.size, ano_str.data, ano_str.size);
    return res;
}

struct str
str_copy(const struct str str)
{
    struct str res;
    res.size = str.size;
    res.data = malloc(sizeof(char) * res.size);
    memcpy(res.data, str.data, res.size);
    return res;
}

void
str_deinit(struct str* str)
{
    free(str->data);
}
