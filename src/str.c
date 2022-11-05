#include "str.h"

/* str buf */

struct str
str_buf_str(const struct str_buf str_buf)
{
    struct str result;
    result.size = str_buf.size;
    result.data = malloc(sizeof(char) * result.size);
    memcpy(result.data, str_buf.data, result.size);
    return result;
}

struct str_buf
str_buf_init(int capacity)
{
    struct str_buf value;
    value.capacity = capacity;
    value.size = 0;
    value.data = malloc(sizeof(char) * value.capacity);
    return value;
}

void
str_buf_deinit(struct str_buf* buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->capacity = 0;
    buf->size = 0;
}

void
str_buf_append(struct str_buf* str_buf, const struct str str)
{
    if (str_buf->capacity < str_buf->size + str.size)
    {
	str_buf->capacity += str.size;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);
    }

    memcpy(str_buf->data + str_buf->size, str.data, str.size);
    str_buf->size += str.size;
}

void
str_buf_insert_char(struct str_buf* str_buf, int at_index, char c)
{
    if (str_buf->capacity < str_buf->size + 1)
    {
	str_buf->capacity += 1;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);  
    }

    memmove(str_buf->data + at_index + 1, str_buf->data + at_index, str_buf->size - at_index + 1);
    str_buf->size++;
    str_buf->data[at_index] = c;
}

void
str_buf_remove_char(struct str_buf* str_buf, int at_index)
{
    memmove(str_buf->data + at_index, str_buf->data + at_index + 1, str_buf->size - at_index);
    str_buf->size--;
}

void
str_buf_resize(struct str_buf* str_buf,
	       int size)
{
    str_buf->size = size;
}

void
str_buf_append_raw(struct str_buf* str_buf, char* string, int len)
{
    if (str_buf->capacity < str_buf->size + len)
    {
	str_buf->capacity += len;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);
    }

    memcpy(str_buf->data + str_buf->size, string, len);
    str_buf->size += len;    
}

/* str */

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
