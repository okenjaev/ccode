#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

struct buffer
{
    char *data;
    int len;
};

#define BUFFER_INIT {NULL, 0}

void
buffer_free(struct buffer *buffer);

void
buffer_append(struct buffer *buffer, const char* s, int len);

#endif /* BUFFER_H */
