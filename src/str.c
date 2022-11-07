#include "str.h"

#define FOR_ME_MEMORY_ALLOCATION_FACTOR 1.5

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
str_buf_append(struct str_buf* str_buf, const struct str_buf str)
{
    if (str_buf->capacity < str_buf->size + str.size)
    {
	str_buf->capacity += str.size * FOR_ME_MEMORY_ALLOCATION_FACTOR;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);
    }

    memcpy(str_buf->data + str_buf->size, str.data, str.size);
    str_buf->size += str.size;
}

void
str_buf_append_str(struct str_buf* str_buf, const struct str str)
{
    if (str_buf->capacity < str_buf->size + str.size)
    {
	str_buf->capacity += str.size * FOR_ME_MEMORY_ALLOCATION_FACTOR;
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
	str_buf->capacity += 5 * FOR_ME_MEMORY_ALLOCATION_FACTOR;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);  
    }

    memmove(str_buf->data + at_index + 1, str_buf->data + at_index, str_buf->size - at_index);
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
    
    if (str_buf->capacity < str_buf->size)
    {
	str_buf->capacity = str_buf->size;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);
    }
}

void
str_buf_append_raw(struct str_buf* str_buf, const char* string, int len)
{
    if (str_buf->capacity < str_buf->size + len)
    {
	str_buf->capacity += len * FOR_ME_MEMORY_ALLOCATION_FACTOR;
	str_buf->data = realloc(str_buf->data, str_buf->capacity);
    }

    memcpy(str_buf->data + str_buf->size, string, len);
    str_buf->size += len;    
}

/* str */

struct str
str_init(char* data, int size)
{
    struct str val;
    val.size = size;
    val.data = malloc(sizeof(char) * val.size);
    memcpy(val.data, data, val.size);
    return val;
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
str_split(struct str* str, const struct str delim)
{
    int delimsize = delim.size;

    int i =0;
    while(i < str->size - delimsize)
    {
	char* at = i + str->data;
	struct str temp = cstrn(at, delimsize);

	if (str_cmp(temp, delim))
	{
	    struct str ret = str_init(str->data, i);
	    str->size = str->size - (i + delimsize);
	    memmove(str->data, i + delimsize + str->data, str->size);	    
	    return ret;
	}
	
	i++;
    }

    return str_error;
}

int
str_cmp(struct str str, struct str ano_str)
{
    if (str.size != ano_str.size)
    {
	return 0;
    }

    for (int i=0; i<str.size; i++)
    {
	if (str.data[i] != ano_str.data[i])
	{
	    return 0;
	}
    }
    
    return 1;
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
    str->data = NULL;
    str->size = 0;
}

#undef FOR_ME_MEMORY_ALLOCATION_FACTOR
